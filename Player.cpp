//
// Created by piotr on 22.01.25.
//

#include "Player.h"

Player::Player(const Board &playerBoard, const Board &opponentBoard, const std::string &name, int socket, void (*sendMessage)(int, const std::string&, bool), std::string (*receiveMessage)(int)) : playerBoard(playerBoard), opponentBoard(opponentBoard), name(name), playerSocket(socket), sendMessage(sendMessage), receiveMessage(receiveMessage) {}

Board& Player::getPlayerBoard() { return playerBoard; }

Board& Player::getOpponentBoard() { return opponentBoard; }

std::string Player::getName() const { return name; }

void Player::setShips(const std::vector<Ship*> ships) {
    this->ships=ships;
}

bool Player::allShipsSunk() const {
    for (const auto& ship : ships) {
        if (!ship->isSunk()) {
            return false;
        }
    }
    return true;
}

Ship* Player::getShip(ShipType shipType) {
    for (auto& ship : ships) {
        if (ship->getShipType() == shipType) {
            return ship;
        }
    }
    return nullptr;
}

void Player::takeShots(Player& opponent) {
    getOpponentBoard().printBoard();
//    std::cout << "---------------------" << std::endl;
    sendMessage(playerSocket, "---------------------\n", false);
    getPlayerBoard().printBoard();

//    std::cout << name << ", it's your turn.";
    sendMessage(playerSocket, name + ", it's your turn.\n", false);

    while (true) {
//        std::cout << "Enter shot coordinates: ";

		sendMessage(playerSocket, "Enter shot coordinates: ", true);
        std::string shot;
        shot = receiveMessage(playerSocket);
        if (shot.empty()) {
        	setConnected(false);
            return;
       	}
        if (shot.length() < 2) {
        	sendMessage(playerSocket, "Invalid shot coordinates. Please use format A0\n", false);
            continue;
        }
//        std::cin >> shot;

//        if (shot == "exit") {
//            break;
//        }


        int row = shot[0] - 'A';
        int col = shot[1] - '0';
        if (row < 0 || row >= 10 || col < 0 || col >= 10) {
//            std::cout << "Invalid shot coordinates. Please use format A0\n";
			sendMessage(playerSocket, "Invalid shot coordinates. Please use format A0\n", false);
            continue;
        }

        Square& opponentSquare = opponent.getPlayerBoard().board[row][col];
        Square& hitSquare = getOpponentBoard().board[row][col];


        if (!hitSquare.isAvailable()) {
//            std::cout << "You already shot here. Please choose another square\n";
			sendMessage(playerSocket, "You already shot here. Please choose another square\n", false);
            continue;
        }

        if (opponentSquare.getSquareValue() == 'O') {
            opponentSquare.setSquareValue('X');
//            opponentSquare.setAvailable(false);
            hitSquare.setSquareValue('X');
            hitSquare.setAvailable(false);
            std::cout << "Hit!\n";
            if (opponent.allShipsSunk()) {
//                std::cout << "All ships sunk! " << name << " wins!\n";
                return;
            } else if (opponent.getShip(opponentSquare.getShipType())->isSunk()) {
//                std::cout << "You sunk a ship!\n";
				sendMessage(playerSocket, "You sunk a ship!\n", false);
            } else {
//                std::cout << "You hit a ship!\n";
				sendMessage(playerSocket, "You hit a ship!\n", false);
            }
        } else {
            opponentSquare.setSquareValue('M');
//            opponentSquare.setAvailable(false);
            hitSquare.setSquareValue('M');
            hitSquare.setAvailable(false);
//            std::cout << "Miss!\n";
            getOpponentBoard().printBoard();
		    sendMessage(playerSocket, "---------------------\n", false);
		    getPlayerBoard().printBoard();
            sendMessage(playerSocket, "Miss!\n", false);

            return;
        }

        getOpponentBoard().printBoard();
//        std::cout << "---------------------" << std::endl;
        sendMessage(playerSocket, "---------------------\n", false);
        getPlayerBoard().printBoard();
    }
}

bool Player::isConnected() const {
	return created;
}

void Player::setConnected(bool state) {
	created = state;
}