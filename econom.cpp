#include "econom.h"

Econom::Econom() : Room() {}

Econom::Econom(int id, int number, double price, int capacity)
    : Room(id, number, price, capacity) {}

QString Econom::getCategory() const { return "Эконом"; }
double Econom::getCategoryCoef() const { return 1.0; }
