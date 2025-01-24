//
// Created by piotr on 22.01.25.
//

#ifndef SQUARE_H
#define SQUARE_H

#pragma once
#include <string>

enum class ShipType;

class Square {
    private:
        std::string squareType;
        ShipType shipType;
        char squareValue;
        bool available;

    public:
        Square(std::string squareType, char squareValue);
        bool isAvailable() const;
        void setAvailable(bool available);
        void setSquareType(std::string squareType);
        char getSquareValue() const;
        void setSquareValue(char squareValue);
        void setShipType(ShipType shipType);
        ShipType getShipType() const;
};


#endif //SQUARE_H
