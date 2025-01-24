//
// Created by piotr on 22.01.25.
//

#pragma once
#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <string>
#include "Square.h"

enum class ShipType {
    CARRIER,
    BATTLESHIP,
    CRUISER,
    SUBMARINE,
    DESTROYER
};

class Ship {
    private:
        int size;
        ShipType shipType;
        std::vector<bool> state;
        std::vector<Square*> sections;
//        std::string beg;
//        std::string end;

   public:
       Ship(int size, ShipType type);
       int getSize() const;
       ShipType getShipType() const;
       bool isSunk() const;
       void setSection(Square* square, int section);
};



#endif //SHIP_H
