//
// Created by piotr on 22.01.25.
//

#include "Square.h"

Square::Square(std::string squareType, char squareValue) : squareType(squareType), squareValue(squareValue), available(true) {}

bool Square::isAvailable() const { return available; }

void Square::setAvailable(bool available) { Square::available = available; }

void Square::setSquareType(std::string squareType) { Square::squareType = squareType; }

char Square::getSquareValue() const { return squareValue; }

void Square::setSquareValue(char squareValue) { Square::squareValue = squareValue; }

void Square::setShipType(ShipType shipType) { Square::shipType = shipType; }

ShipType Square::getShipType() const { return shipType; }
