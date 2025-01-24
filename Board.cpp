//
// Created by piotr on 22.01.25.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Board.h"

Board::Board(int socket, void (*sendMessage)(int, const std::string&, bool), std::string (*receiveMessage)(int)) : board(10, std::vector<Square>(10, Square("Empty", '~'))), playerSocket(socket), sendMessage(sendMessage), receiveMessage(receiveMessage) {}

void Board::printBoard() const {
  	std::string message;
    for (int i = -1; i < 10; i++) {
        if (i == -1) {
            message.append(" ");
        } else {
            message.append(" " + std::to_string(i));
        }
    }
    message.append("\n");

    for (int i = 0; i < 10; i++) {
        message.append(std::string(1, char('A' + i)));
        for (int j = 0; j < 10; j++) {
            message.append(" " + std::string(1, board[i][j].getSquareValue()));
        }
    	message.append("\n");
    }

    sendMessage(playerSocket, message, false);
}


void Board::placeShips(std::vector<Ship*> ships) {
    printBoard();
	std::string message;
	std::string userInput;

    std::map<ShipType, std::string> shipTypes = {
        {ShipType::CARRIER, "Carrier"},
        {ShipType::BATTLESHIP, "Battleship"},
        {ShipType::CRUISER, "Crusier"},
        {ShipType::SUBMARINE, "Submarine"},
        {ShipType::DESTROYER, "Destroyer"}
    };


    for (auto ship : ships) {
        std::string beg;
        std::string end;
        bool placed = false;
        while (!placed) {
            message = "Place " + shipTypes[ship->getShipType()] + " (size: " + std::to_string(ship->getSize()) + "): ";
            sendMessage(playerSocket, message, true);
			userInput = receiveMessage(playerSocket);
			if (userInput.empty()) {
            	created = false;
                return;
            }
            if (userInput.length() < 6) {
                sendMessage(playerSocket, "Invalid coordinates. Please use format: A0 A4\n", false);
                continue;
            }
            beg = userInput.substr(0, 2);
            end = userInput.substr(3, 2);
            if (placeShip(beg, end, *ship)) {
                placed = true;
            } else {
            	sendMessage(playerSocket, "Invalid coordinates. Please use format: A0 A4\n", false);
            }
        }
        printBoard();
    }
    created = true;
}


bool Board::placeShip(const std::string& beg, const std::string& end, Ship& ship) {
    std::map<ShipType, std::string> shipTypes = {
        {ShipType::CARRIER, "Carrier"},
        {ShipType::BATTLESHIP, "Battleship"},
        {ShipType::CRUISER, "Crusier"},
        {ShipType::SUBMARINE, "Submarine"},
        {ShipType::DESTROYER, "Destroyer"}
    };

    int begRow = beg[0] - 'A';
    int begCol = beg[1] - '0';
    int endRow = end[0] - 'A';
    int endCol = end[1] - '0';

    if (begRow > endRow) {
        std::swap(begRow, endRow);
    }
    if (begCol > endCol) {
        std::swap(begCol, endCol);
    }

    // Check if placement is valid
    if (begRow < 0 || begRow >= 10 || endRow < 0 || endRow >= 10 || begCol < 0 || begCol >= 10 || endCol < 0 || endCol >= 10) {
        return false;
    }
    if (begRow != endRow && begCol != endCol) {
        return false;
    }
    if (begRow == endRow && endCol - begCol + 1 != ship.getSize()) {
        return false;
    }
    if (begCol == endCol && endRow - begRow + 1 != ship.getSize()) {
        return false;
    }

    // Place ship
    if (begRow == endRow) {
        // check if ship is not to close to another
        for (int i = begCol; i <= endCol; i++) {
            if (!board[begRow][i].isAvailable()) {
                return false;
            }
        }

        // place ship
        for (int i = begCol; i <= endCol; i++) {
            board[begRow][i].setSquareType("Ship");
            board[begRow][i].setSquareValue('O');
            board[begRow][i].setAvailable(false);
            board[begRow][i].setShipType(ship.getShipType());
            ship.setSection(&board[begRow][i], i - begCol);
        }

        // lock squares arround ship
        for (int i = begRow - 1; i <= endRow + 1; i++) {
            for (int j = begCol - 1; j <= endCol + 1; j++) {
                if (i >= 0 && i < 10 && j >= 0 && j < 10) {
                    board[i][j].setAvailable(false);
                    board[i][j].setSquareType("ArroundShip");
                }
            }
        }
    } else {
        for (int i = begRow; i <= endRow; i++) {
            if (!board[i][begCol].isAvailable()) {
                return false;
            }
        }

        for (int i = begRow; i <= endRow; i++) {
            board[i][begCol].setSquareType("Ship");
            board[i][begCol].setSquareValue('O');
            board[i][begCol].setAvailable(false);
            board[i][begCol].setShipType(ship.getShipType());
            ship.setSection(&board[i][begCol], i - begRow);
        }

        for (int i = begRow - 1; i <= endRow + 1; i++) {
            for (int j = begCol - 1; j <= endCol + 1; j++) {
                if (i >= 0 && i < 10 && j >= 0 && j < 10) {
                    board[i][j].setAvailable(false);
                    board[i][j].setSquareType("ArroundShip");
                }
            }
        }
    }
    return true;
}

bool Board::isCreated() const {
	return created;
}