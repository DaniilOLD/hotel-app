#ifndef ROOMFACTORY_H
#define ROOMFACTORY_H

#include "room.h"
#include <QString>

// Паттерн FACTORY METHOD
class RoomFactory
{
public:
    enum class Category { Econom, Standard, Lux };

    static Room* createRoom(Category category,
                            int id, int number,
                            double price, int capacity);

    static Room* createRoom(const QString& categoryName,
                            int id, int number,
                            double price, int capacity);
};


#endif // ROOMFACTORY_H
