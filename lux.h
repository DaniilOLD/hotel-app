#ifndef LUX_H
#define LUX_H

#include "room.h"

class Lux : public Room
{
public:
    Lux();
    Lux(int id, int number, double price, int capacity);

    QString getCategory() const override;    // "Люкс"
    double getCategoryCoef() const override; // 2.5
};


#endif // LUX_H
