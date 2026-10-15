#include "hotel.h"
#include <algorithm>

// --- SINGLETON ---
Hotel& Hotel::instance() {
    static Hotel instance;
    return instance;
}

// --- FACADE ---
void Hotel::addGuest(Guest* guest) {
    if (guest) m_guests.push_back(guest);
}

void Hotel::removeGuest(int id) {
    m_guests.erase(
        std::remove_if(m_guests.begin(), m_guests.end(),
                       [id](Guest* g) { return g->getId() == id; }),
        m_guests.end());
}

const std::vector<Guest*>& Hotel::getGuests() const { return m_guests; }

void Hotel::addRoom(Room* room) {
    if (room) m_rooms.push_back(room);
}

void Hotel::removeRoom(int id) {
    m_rooms.erase(
        std::remove_if(m_rooms.begin(), m_rooms.end(),
                       [id](Room* r) { return r->getId() == id; }),
        m_rooms.end());
}

const std::vector<Room*>& Hotel::getRooms() const { return m_rooms; }

void Hotel::addFeature(Feature* feature) {
    if (feature) m_features.push_back(feature);
}

void Hotel::addPreference(Preference* preference) {
    if (preference) m_preferences.push_back(preference);
}

const std::vector<Feature*>& Hotel::getFeatures() const { return m_features; }
const std::vector<Preference*>& Hotel::getPreferences() const { return m_preferences; }

void Hotel::addBooking(Booking* booking) {
    if (booking) m_bookings.push_back(booking);
}

void Hotel::removeBooking(int id) {
    m_bookings.erase(
        std::remove_if(m_bookings.begin(), m_bookings.end(),
                       [id](Booking* b) { return b->getId() == id; }),
        m_bookings.end());
}

const std::vector<Booking*>& Hotel::getBookings() const { return m_bookings; }

std::vector<Room*> Hotel::selectRoomsForGuest(Guest* guest, int capacity) const {
    std::vector<Room*> result;
    for (Room* room : m_rooms) {
        if (room->getStatus() != RoomStatus::Free) continue;
        if (room->getCapacity() < capacity) continue;
        result.push_back(room);
    }
    return result;
}

std::vector<Room*> Hotel::getFreeRooms() const {
    std::vector<Room*> result;
    for (Room* r : m_rooms)
        if (r->getStatus() == RoomStatus::Free)
            result.push_back(r);
    return result;
}

std::vector<Guest*> Hotel::getCurrentGuests() const {
    std::vector<Guest*> result;
    for (Booking* b : m_bookings)
        if (b->getGuest())
            result.push_back(b->getGuest());
    return result;
}