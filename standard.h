#ifndef STANDARD_H
#define STANDARD_H

#include "room.h"

class Standard : public Room
{
public:
    Standard();
    Standard(int id, int number, double price, int capacity);

    QString getCategory() const override;    // "Стандарт"
    double getCategoryCoef() const override; // 1.5
};


#endif // STANDARD_H
