//
// Created by piotr on 22.01.25.
//

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Board.h"
#include "Ship.h"

class Player {
    private:
        std::string name;
        Board playerBoard;
        Board opponentBoard;
        std::vector<Ship*> ships;
        int playerSocket;
        void (*sendMessage)(int, const std::string&, bool);
        std::string (*receiveMessage)(int);
        bool created;

    public:
        Player(const Board& playerBoard, const Board& opponentBoard, const std::string& name, int socket, void (*sendMessage)(int, const std::string&, bool), std::string (*receiveMessage)(int));

        ~Player() {  // Destructor moved inside class
            for (Ship* ship : ships) {
                delete ship;
            }
            ships.clear();
        }

        Board& getPlayerBoard();
        Board& getOpponentBoard();
        std::string getName() const;
        void setShips(std::vector<Ship*> ships);
        bool allShipsSunk() const;
        Ship* getShip(ShipType shipType);
        void takeShots(Player& opponent);
        bool isConnected() const;
        void setConnected(bool state);
};


#endif //PLAYER_H
