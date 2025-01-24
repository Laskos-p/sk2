//
// Created by piotr on 22.01.25.
//

#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include "Square.h"
#include "Ship.h"


class Board {
    private:
        int playerSocket;
        void (*sendMessage)(int, const std::string&, bool);
        std::string (*receiveMessage)(int);
        bool created;

    public:
        std::vector<std::vector<Square>> board;
        Board(int socket, void (*sendMessage)(int, const std::string&, bool), std::string (*receiveMessage)(int));
        // Board();
        void printBoard() const;
        void placeShips(std::vector<Ship*> ships);
        bool placeShip(const std::string& beg, const std::string& end, Ship& ship);
        bool isCreated() const;
};

#endif //BOARD_H
