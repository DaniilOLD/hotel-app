#include "booking.h"
Booking::Booking()
    : m_id(0), m_guest(nullptr), m_room(nullptr) {}

Booking::Booking(int id, Guest* guest, Room* room,
                 const QDate& checkIn, const QDate& checkOut)
    : m_id(id), m_guest(guest), m_room(room),
    m_checkIn(checkIn), m_checkOut(checkOut) {}

int Booking::getId() const { return m_id; }
Guest* Booking::getGuest() const { return m_guest; }
Room* Booking::getRoom() const { return m_room; }
QDate Booking::getCheckIn() const { return m_checkIn; }
QDate Booking::getCheckOut() const { return m_checkOut; }

void Booking::setId(int id) { m_id = id; }
void Booking::setGuest(Guest* guest) { m_guest = guest; }
void Booking::setRoom(Room* room) { m_room = room; }
void Booking::setCheckIn(const QDate& date) { m_checkIn = date; }
void Booking::setCheckOut(const QDate& date) { m_checkOut = date; }

int Booking::getNights() const {
    return m_checkIn.daysTo(m_checkOut);
}