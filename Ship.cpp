//
// Created by piotr on 22.01.25.
//

#include "Ship.h"
#include <iostream>

Ship::Ship(int size, ShipType type) : size(size), shipType(type), sections(size, nullptr) {}

int Ship::getSize() const { return size; }
ShipType Ship::getShipType() const { return shipType; }

bool Ship::isSunk() const {
    for (const auto& section : sections) {
        if (section->getSquareValue() == 'O') {
            return false;
        }
    }
    return true;
}

//bool Ship::isSunk() const {
//    std::cout << "Checking if ship is sunk\n";
//    std::cout << sections.size() << std::endl;
//    for (Square* section : sections) {
//      	std::cout << "Checking section\n";
//        std::cout << section->getSquareValue() << std::endl;
//        if (section == nullptr) { // Null check
//            std::cerr << "Error: Null pointer in ship sections\n";
//            return false; // Or handle the error as needed
//        }
//        if (section->getSquareValue() == 'O') {
//            std::cout << "Ship is not yet sunk\n";
//            return false; // Ship is not yet sunk
//        }
//        std::cout << "Section is sunk\n";
//    }
//    std::cout << "Ship is sunk\n";
//    return true;
//}


void Ship::setSection(Square* square, int section) {
    if (section >= 0 && section < size) {
        sections[section] = square;
    }
}

