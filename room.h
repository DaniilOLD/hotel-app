#ifndef ROOM_H
#define ROOM_H

#include "feature.h"
#include <QString>
#include <vector>

// Статус номера
enum class RoomStatus {
    Free,       // свободен
    Occupied,   // занят

};

// Номер гостиницы — базовый класс
class Room
{
public:
    Room();
    Room(int id, int number, double price, int capacity);
    virtual ~Room() = default;

    int getId() const;
    int getNumber() const;
    double getPrice() const;
    int getCapacity() const;
    RoomStatus getStatus() const;

    void setId(int id);
    void setNumber(int number);
    void setPrice(double price);
    void setCapacity(int capacity);
    void setStatus(RoomStatus status);


    void addFeature(Feature* feature);
    void removeFeature(int featureId);
    const std::vector<Feature*>& getFeatures() const;


    virtual QString getCategory() const = 0;
    virtual double getCategoryCoef() const = 0;

protected:
    int m_id;
    int m_number;
    double m_price;
    int m_capacity;
    RoomStatus m_status;
    std::vector<Feature*> m_features;   // ← МЗ
};


#endif // ROOM_H
