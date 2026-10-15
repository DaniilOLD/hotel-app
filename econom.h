#ifndef ECONOM_H
#define ECONOM_H

#include "room.h"

class Econom : public Room
{
public:
    Econom();
    Econom(int id, int number, double price, int capacity);

    QString getCategory() const override;    // "Эконом"
    double getCategoryCoef() const override; // 1.0
};

#endif // ECONOM_H
