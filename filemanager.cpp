#include "filemanager.h"
#include "hotel.h"
#include "room.h"
#include "roomfactory.h"
#include "guest.h"
#include "feature.h"
#include "preference.h"
#include "booking.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

// ============================================================
// Путь к файлу по умолчанию
// ============================================================
QString FileManager::defaultFilename()
{
    return "hotel_data.txt";
}

// ============================================================
// save() — сохранить всё в файл
// ============================================================
bool FileManager::save(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл для записи:" << filename;
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    Hotel& h = Hotel::instance();

    // --- Удобства ---
    out << "[FEATURES]\n";
    for (Feature* f : h.getFeatures()) {
        out << f->getId() << "|" << f->getName() << "\n";
    }
    out << "\n";

    // --- Пожелания ---
    out << "[PREFERENCES]\n";
    for (Preference* p : h.getPreferences()) {
        out << p->getId() << "|" << p->getName() << "\n";
    }
    out << "\n";

    // --- Номера ---
    out << "[ROOMS]\n";
    for (Room* r : h.getRooms()) {
        // Удобства через запятую
        QString features;
        for (Feature* f : r->getFeatures()) {
            if (!features.isEmpty()) features += ",";
            features += QString::number(f->getId());
        }

        // Статус
        QString status = (r->getStatus() == RoomStatus::Free) ? "Free" : "Occupied";

        out << r->getId() << "|"
            << r->getNumber() << "|"
            << r->getCategory() << "|"
            << r->getPrice() << "|"
            << r->getCapacity() << "|"
            << status << "|"
            << features << "\n";
    }
    out << "\n";

    // --- Гости ---
    out << "[GUESTS]\n";
    for (Guest* g : h.getGuests()) {
        // Пожелания через запятую
        QString prefs;
        for (Preference* p : g->getPreferences()) {
            if (!prefs.isEmpty()) prefs += ",";
            prefs += QString::number(p->getId());
        }

        out << g->getId() << "|"
            << g->getFullName() << "|"
            << g->getPhone() << "|"
            << prefs << "\n";
    }
    out << "\n";

    // --- Брони ---
    out << "[BOOKINGS]\n";
    for (Booking* b : h.getBookings()) {
        if (!b->getGuest() || !b->getRoom()) continue;

        out << b->getId() << "|"
            << b->getGuest()->getId() << "|"
            << b->getRoom()->getId() << "|"
            << b->getCheckIn().toString("dd.MM.yyyy") << "|"
            << b->getCheckOut().toString("dd.MM.yyyy") << "\n";
    }

    file.close();
    qDebug() << "Сохранено в:" << filename;
    return true;
}

// ============================================================
// load() — загрузить всё из файла
// ============================================================
bool FileManager::load(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Файл не найден, начинаем с пустой базы:" << filename;
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    Hotel& h = Hotel::instance();

    QString section;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty()) continue;

        // --- Определяем секцию ---
        if (line.startsWith("[")) {
            section = line.mid(1, line.length() - 2);
            continue;
        }

        QStringList parts = line.split("|");

        // --- FEATURES ---
        if (section == "FEATURES" && parts.size() >= 2) {
            int id = parts[0].toInt();
            QString name = parts[1];
            h.addFeature(new Feature(id, name));
        }
        // --- PREFERENCES ---
        else if (section == "PREFERENCES" && parts.size() >= 2) {
            int id = parts[0].toInt();
            QString name = parts[1];
            h.addPreference(new Preference(id, name));
        }
        // --- ROOMS ---
        else if (section == "ROOMS" && parts.size() >= 7) {
            int id = parts[0].toInt();
            int number = parts[1].toInt();
            QString category = parts[2];
            double price = parts[3].toDouble();
            int capacity = parts[4].toInt();
            QString statusStr = parts[5];
            QString featuresStr = parts[6];

            // Создаём номер через фабрику
            Room* r = nullptr;
            if (category == "Эконом")
                r = RoomFactory::createRoom(RoomFactory::Category::Econom, id, number, price, capacity);
            else if (category == "Стандарт")
                r = RoomFactory::createRoom(RoomFactory::Category::Standard, id, number, price, capacity);
            else if (category == "Люкс")
                r = RoomFactory::createRoom(RoomFactory::Category::Lux, id, number, price, capacity);

            if (!r) continue;

            // Статус
            r->setStatus(statusStr == "Free" ? RoomStatus::Free : RoomStatus::Occupied);

            // Удобства
            if (!featuresStr.isEmpty()) {
                QStringList featureIds = featuresStr.split(",");
                for (const QString& fidStr : featureIds) {
                    int fid = fidStr.toInt();
                    for (Feature* f : h.getFeatures()) {
                        if (f->getId() == fid) {
                            r->addFeature(f);
                            break;
                        }
                    }
                }
            }

            h.addRoom(r);
        }
        // --- GUESTS ---
        else if (section == "GUESTS" && parts.size() >= 4) {
            int id = parts[0].toInt();
            QString fullName = parts[1];
            QString phone = parts[2];
            QString prefsStr = parts[3];

            Guest* g = new Guest(id, fullName, phone);

            // Пожелания
            if (!prefsStr.isEmpty()) {
                QStringList prefIds = prefsStr.split(",");
                for (const QString& pidStr : prefIds) {
                    int pid = pidStr.toInt();
                    for (Preference* p : h.getPreferences()) {
                        if (p->getId() == pid) {
                            g->addPreference(p);
                            break;
                        }
                    }
                }
            }

            h.addGuest(g);
        }
        // --- BOOKINGS ---
        else if (section == "BOOKINGS" && parts.size() >= 5) {
            int id = parts[0].toInt();
            int guestId = parts[1].toInt();
            int roomId = parts[2].toInt();
            QDate checkIn = QDate::fromString(parts[3], "dd.MM.yyyy");
            QDate checkOut = QDate::fromString(parts[4], "dd.MM.yyyy");

            Guest* guest = nullptr;
            for (Guest* g : h.getGuests())
                if (g->getId() == guestId) { guest = g; break; }

            Room* room = nullptr;
            for (Room* r : h.getRooms())
                if (r->getId() == roomId) { room = r; break; }

            if (guest && room) {
                h.addBooking(new Booking(id, guest, room, checkIn, checkOut));
            }
        }
    }

    file.close();
    qDebug() << "Загружено из:" << filename;
    return true;
}