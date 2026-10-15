#include "standard.h"
Standard::Standard() : Room() {}

Standard::Standard(int id, int number, double price, int capacity)
    : Room(id, number, price, capacity) {}

QString Standard::getCategory() const { return "Стандарт"; }
double Standard::getCategoryCoef() const { return 1.5; }