#ifndef HOTEL_H
#define HOTEL_H

#include "guest.h"
#include "room.h"
#include "booking.h"
#include "feature.h"
#include "preference.h"
#include <vector>

// Управляющий класс — Singleton + Facade
class Hotel
{
public:
    static Hotel& instance();   // ← Singleton

    // Гости
    void addGuest(Guest* guest);
    void removeGuest(int id);
    const std::vector<Guest*>& getGuests() const;

    // Номера
    void addRoom(Room* room);
    void removeRoom(int id);
    const std::vector<Room*>& getRooms() const;

    // Справочники
    void addFeature(Feature* feature);
    void addPreference(Preference* preference);
    const std::vector<Feature*>& getFeatures() const;
    const std::vector<Preference*>& getPreferences() const;

    // Бронирования
    void addBooking(Booking* booking);
    void removeBooking(int id);
    const std::vector<Booking*>& getBookings() const;

    // ЯДРО: подбор номера под гостя
    std::vector<Room*> selectRoomsForGuest(Guest* guest, int capacity) const;

    // Отчёты
    std::vector<Room*> getFreeRooms() const;
    std::vector<Guest*> getCurrentGuests() const;

private:
    Hotel() = default;
    Hotel(const Hotel&) = delete;
    Hotel& operator=(const Hotel&) = delete;

    std::vector<Guest*> m_guests;
    std::vector<Room*> m_rooms;
    std::vector<Booking*> m_bookings;
    std::vector<Feature*> m_features;
    std::vector<Preference*> m_preferences;
};


#endif // HOTEL_H
