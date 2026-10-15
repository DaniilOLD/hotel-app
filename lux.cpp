#include "lux.h"
Lux::Lux() : Room() {}

Lux::Lux(int id, int number, double price, int capacity)
    : Room(id, number, price, capacity) {}

QString Lux::getCategory() const { return "Люкс"; }
double Lux::getCategoryCoef() const { return 2.5; }
