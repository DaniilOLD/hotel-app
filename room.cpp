#include "room.h"
#include <algorithm>

Room::Room()
    : m_id(0), m_number(0), m_price(0), m_capacity(1), m_status(RoomStatus::Free) {}

Room::Room(int id, int number, double price, int capacity)
    : m_id(id), m_number(number), m_price(price),
    m_capacity(capacity), m_status(RoomStatus::Free) {}

int Room::getId() const { return m_id; }
int Room::getNumber() const { return m_number; }
double Room::getPrice() const { return m_price; }
int Room::getCapacity() const { return m_capacity; }
RoomStatus Room::getStatus() const { return m_status; }

void Room::setId(int id) { m_id = id; }
void Room::setNumber(int number) { m_number = number; }
void Room::setPrice(double price) { m_price = price; }
void Room::setCapacity(int capacity) { m_capacity = capacity; }
void Room::setStatus(RoomStatus status) { m_status = status; }

// --- МНОГОЗНАЧНАЯ ЗАВИСИМОСТЬ ---
void Room::addFeature(Feature* feature) {
    if (feature) m_features.push_back(feature);
}

void Room::removeFeature(int featureId) {
    m_features.erase(
        std::remove_if(m_features.begin(), m_features.end(),
                       [featureId](Feature* f) { return f->getId() == featureId; }),
        m_features.end());
}

const std::vector<Feature*>& Room::getFeatures() const {
    return m_features;
}