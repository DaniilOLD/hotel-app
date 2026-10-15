#include "roomfactory.h"
#include "econom.h"
#include "standard.h"
#include "lux.h"

Room* RoomFactory::createRoom(Category category,
                              int id, int number,
                              double price, int capacity)
{
    switch (category) {
    case Category::Econom:   return new Econom(id, number, price, capacity);
    case Category::Standard: return new Standard(id, number, price, capacity);
    case Category::Lux:      return new Lux(id, number, price, capacity);
    }
    return nullptr;
}

Room* RoomFactory::createRoom(const QString& categoryName,
                              int id, int number,
                              double price, int capacity)
{
    if (categoryName == "Эконом")   return createRoom(Category::Econom,   id, number, price, capacity);
    if (categoryName == "Стандарт") return createRoom(Category::Standard, id, number, price, capacity);
    if (categoryName == "Люкс")     return createRoom(Category::Lux,      id, number, price, capacity);
    return nullptr;
}
