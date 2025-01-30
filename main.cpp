#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>
#include <map>
#include <mutex>
#include <memory>
#include "Board.h"
#include "Player.h"

#define PORT 1100
#define BUFFER_SIZE 1024

std::mutex room_mutex;
std::map<int, std::pair<int, int>> rooms;

std::string receiveMessage(int clientSocket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int recv_code = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recv_code == -1) {
        perror("Error while receiving message");
        return "";
    } else if (recv_code == 0) {
        std::cout << "Client disconnected: socket " << clientSocket << std::endl;
        return "";
    }
    return std::string(buffer);
}

void sendMessage(int clientSocket, const std::string &message, bool sendRequest = false) {
    char buffer[BUFFER_SIZE];
    uint32_t msgLength = htonl(message.size());
    if (send(clientSocket, &msgLength, sizeof(msgLength), 0) == -1) {
        perror("Error while sending message");
    }


    if (send(clientSocket, message.c_str(), message.size(), 0) == -1) {
        perror("Error while sending message");
    }

    // send request to client to send message
    //memset(buffer, 0, sizeof(buffer));
    std::string requestMessage;
    if (sendRequest) {
        requestMessage = "SEND SOMETHING";
        msgLength = htonl(requestMessage.size());
        if (send(clientSocket, &msgLength, sizeof(msgLength), 0) == -1) {
    	    perror("Error while sending message");
    	}
        if (send(clientSocket, requestMessage.c_str(), requestMessage.size(), 0) == -1) {
            perror("Error while sending request for message");

        }
    } else {
        requestMessage = "NO";
        msgLength = htonl(requestMessage.size());
        if (send(clientSocket, &msgLength, sizeof(msgLength), 0) == -1) {
    	    perror("Error while sending message");
    	}
        if (send(clientSocket, requestMessage.c_str(), requestMessage.size(), 0) == -1) {
            perror("Error while sending request for message");
        }
    }
}

Player createPlayer(int playerSocket, int opponentSocket, const std::string &name) {
    Ship* carrier = new Ship(5, ShipType::CARRIER);
    Ship* battleship = new Ship(4, ShipType::BATTLESHIP);
    Ship* cruiser = new Ship(3, ShipType::CRUISER);
    Ship* submarine = new Ship(3, ShipType::SUBMARINE);
    Ship* destroyer = new Ship(2, ShipType::DESTROYER);

    std::vector<Ship*> ships;
    ships.push_back(carrier);
    ships.push_back(battleship);
    ships.push_back(cruiser);
    ships.push_back(submarine);
    ships.push_back(destroyer);

    Board playerBoard(playerSocket, sendMessage, receiveMessage);
    Board opponentBoard(playerSocket, sendMessage, receiveMessage);

    Player player(playerBoard, opponentBoard, name, playerSocket, sendMessage, receiveMessage);

    player.getPlayerBoard().placeShips(ships);
    player.setConnected(player.getPlayerBoard().isCreated());

    player.setShips(ships);

    return player;
}

void handleRoom(int playerSocket1, int playerSocket2) {
    std::string message;
    char buffer[BUFFER_SIZE];
    std::string player1name;
    std::string player2name;

    message = "You successfully connected to the game room.\n"
              "Game started\n\n";
    sendMessage(playerSocket1, message);
    sendMessage(playerSocket2, message);

    // Ask for names
    sendMessage(playerSocket2, "Wait for your opponent to give his name\n");
    sendMessage(playerSocket1, "Enter your name: ", true);
    player1name = receiveMessage(playerSocket1);
    if (player1name.empty()) {
        close(playerSocket1);
        close(playerSocket2);
        return;
    }
    if (!player1name.empty() && player1name.back() == '\n') {
        player1name.pop_back();
    }

    sendMessage(playerSocket1, "Wait for your opponent to give his name\n");
    sendMessage(playerSocket2, "Enter your name: ", true);
    player2name = receiveMessage(playerSocket2);
    if (player2name.empty()) {
        close(playerSocket1);
        close(playerSocket2);
        return;
    }
    if (!player2name.empty() && player2name.back() == '\n') {
        player2name.pop_back();
    }

    // create players
    sendMessage(playerSocket2, "Wait for " + player1name + " to place his ships\n");
    sendMessage(playerSocket1, "\nPlace your ships\n\n");
    Player player1 = createPlayer(playerSocket1, playerSocket2, player1name);
    if (!player1.isConnected()) {
        sendMessage(playerSocket2, player1name + " left\n");
        close(playerSocket1);
        close(playerSocket2);
        return;
    }

    sendMessage(playerSocket1, "\nWait for " + player2name + " to place his ships\n");
    sendMessage(playerSocket2, "\nPlace your ships\n\n");
    Player player2 = createPlayer(playerSocket2, playerSocket1, player2name);
    if (!player1.isConnected()) {
        sendMessage(playerSocket2, player1name + " left\n");
        close(playerSocket1);
        close(playerSocket2);
        return;
    }
    if (!player2.isConnected()) {
        sendMessage(playerSocket1, player2name + " left\n");
        close(playerSocket1);
        close(playerSocket2);
        return;
    }

    while (true) {
        sendMessage(playerSocket2, "Wait for " + player1name + " to take shots\n");
        player1.takeShots(player2);
        if (!player1.isConnected()) {
            sendMessage(playerSocket2, player1name + " left\n");
            close(playerSocket1);
            close(playerSocket2);
            return;
        }
        if (!player2.isConnected()) {
            sendMessage(playerSocket1, player2name + " left\n");
            close(playerSocket1);
            close(playerSocket2);
            return;
        }

        if (player2.allShipsSunk()) {
            sendMessage(playerSocket1, "You won!\n");
            sendMessage(playerSocket2, "You lost!\n");
            break;
        }

        sendMessage(playerSocket1, "Wait for " + player2name + " to take shots\n");
        player2.takeShots(player1);

        if (!player1.isConnected()) {
            sendMessage(playerSocket2, player1name + " left\n");
            close(playerSocket1);
            close(playerSocket2);
            return;
        }
        if (!player2.isConnected()) {
            sendMessage(playerSocket1, player2name + " left\n");
            close(playerSocket1);
            close(playerSocket2);
            return;
        }

        if (player1.allShipsSunk()) {
            sendMessage(playerSocket2, "You won!\n");
            sendMessage(playerSocket1, "You lost!\n");
            break;
        }
    }

    message ="Thanks for playing!\n"
            "Start the client one more time to play again.\n";
    sendMessage(playerSocket1, message);
    sendMessage(playerSocket2, message);
    close(playerSocket1);
    close(playerSocket2);
}

void createGame(int playerSocket) {
    // create a new game and connect the player to it
    sendMessage(playerSocket, "Created room\n"
                              "Waiting for other player to join\n");

    std::unique_lock<std::mutex> lock(room_mutex);
    rooms[playerSocket] = std::make_pair(playerSocket, -1);
    lock.unlock();

}

int joinGame(int playerSocket) {
    // join player to existing game
    // sendMessage(playerSocket, "Clicked join game\n");
    // bool connected = false;

    std::unique_lock<std::mutex> lock(room_mutex);
    for (auto &room : rooms) {
        if (room.second.second == -1) {
            room.second.second = playerSocket;
            sendMessage(room.second.first, "Player 2 joined the game\n");
            std::thread roomThread(handleRoom, room.second.first, room.second.second);
            roomThread.detach();
            // sendMessage(room.second.second, "Player 2 joined the game\n");

            // connected = true;
            lock.unlock();


            return 0;
        }
    }
    lock.unlock();
    return -1;
}

void mainMenu(int playerSocket) {
    char buffer[BUFFER_SIZE];
    std::string message;
    char options[3] = {'1', '2', '3'};

    while (true) {
        // Show main menu
        message = "\n1. Join game\n"
              "2. Create game\n"
              "3. Exit\n";
        sendMessage(playerSocket, message, true);

        // Receive option from client
        memset(buffer, 0, sizeof(buffer));
        int recv_code = recv(playerSocket, buffer, sizeof(buffer), 0);
        if (recv_code == -1) {
            perror("Error while receiving message");
            return;
        } else if (recv_code == 0) {
            std::cout << "Client disconnected: socket " << playerSocket << std::endl;
            return;
        }
        // recv(playerSocket, buffer, sizeof(buffer), 0);

        // remove newline character
        char *newline = strchr(buffer, '\n');
        if (newline) {
            *newline = '\0';
        }

        // handle options
        if (buffer[0] == options[0]) {
            if (joinGame(playerSocket) == -1) {
                message = "No games available. Create your own game.\n";
                sendMessage(playerSocket, message);
            } else {
                break;
            }
            // joinGame(playerSocket);
            // std::cout << "Join game\n";
            // break;
        } else if (buffer[0] == options[1]) {

            createGame(playerSocket);
            // std::cout << "Create game\n";
            break;
        } else if (buffer[0] == options[2]) {
            message = "Disconnecting...\n";
            sendMessage(playerSocket, message);
            std::cout << "Disconnected player " << playerSocket << std::endl;
            close(playerSocket);
            break;
        } else {
            message = "Invalid option. Please try again.\n";
            sendMessage(playerSocket, message);
        }
    }
}

void handlePlayer(int playerSocket) {
    std::string message;

    // send welcome message
    message = "You successfully connected to the battleships server.\n";
              // "Press [Enter] to continue...\n";
    sendMessage(playerSocket, message);

    mainMenu(playerSocket);

    // close(playerSocket);
}


int main() {
    int serverSocket;
    sockaddr_in serverAddr{}, clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error while creating socket");
        return EXIT_FAILURE;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);


	int opt = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // Bind socket to the address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error while binding socket");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    // Start listening for connections
    if (listen(serverSocket, 2) == -1) {
        perror("Error while listening on socket");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept and handle clients in a loop
    std::vector<std::thread> threads;
    while (true) {
        // Accept client socket
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            perror("Error while accepting connection");
            continue;
        }
        std::cout << "Client connected: "
                  << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;


        threads.emplace_back(std::thread(handlePlayer, clientSocket));
        threads.back().detach();

    }

    for (auto &thread : threads) {
        thread.join();
    }

    // Close the server socket
    close(serverSocket);
    return 0;
}
