#ifndef BOOKING_H
#define BOOKING_H

#include "guest.h"
#include "room.h"
#include <QDate>

// Бронирование: гость + номер + даты
class Booking
{
public:
    Booking();
    Booking(int id, Guest* guest, Room* room,
            const QDate& checkIn, const QDate& checkOut);

    int getId() const;
    Guest* getGuest() const;
    Room* getRoom() const;
    QDate getCheckIn() const;
    QDate getCheckOut() const;

    void setId(int id);
    void setGuest(Guest* guest);
    void setRoom(Room* room);
    void setCheckIn(const QDate& date);
    void setCheckOut(const QDate& date);

    int getNights() const;

private:
    int m_id;
    Guest* m_guest;
    Room* m_room;
    QDate m_checkIn;
    QDate m_checkOut;
};

#endif // BOOKING_H
