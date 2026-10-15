#include "initializer.h"

#include "hotel.h"
#include "feature.h"
#include "preference.h"
#include "room.h"
#include "roomfactory.h"

#include <QDebug>

// ============================================================
// initData() — заполняет базу при старте программы
// ============================================================
void initData()
{
    if (!Hotel::instance().getRooms().empty()) {
        qDebug() << "Данные уже загружены, пропускаем инициализацию";
        return;
    }
    // ============================================================
    // 1. Удобства (5 штук)
    // ============================================================
    Hotel::instance().addFeature(new Feature(1, "Wi-Fi"));
    Hotel::instance().addFeature(new Feature(2, "Телевизор"));
    Hotel::instance().addFeature(new Feature(3, "Кондиционер"));
    Hotel::instance().addFeature(new Feature(4, "Мини-бар"));
    Hotel::instance().addFeature(new Feature(5, "Джакузи"));

    // ============================================================
    // 2. Пожелания — ТЕ ЖЕ, что удобства (5 штук)
    //    Это нужно, чтобы стратегия подбора работала:
    //    пожелание гостя → удобство номера
    // ============================================================
    Hotel::instance().addPreference(new Preference(1, "Wi-Fi"));
    Hotel::instance().addPreference(new Preference(2, "Телевизор"));
    Hotel::instance().addPreference(new Preference(3, "Кондиционер"));
    Hotel::instance().addPreference(new Preference(4, "Мини-бар"));
    Hotel::instance().addPreference(new Preference(5, "Джакузи"));

    // ============================================================
    // 3. Ссылки на удобства (для создания номеров)
    // ============================================================
    const auto& features = Hotel::instance().getFeatures();
    Feature* wiFi        = features[0];
    Feature* tv          = features[1];
    Feature* conditioner = features[2];
    Feature* minibar     = features[3];
    Feature* jacuzzi     = features[4];

    // ============================================================
    // 4. 5 номеров "Эконом" (101–105)
    // ============================================================
    for (int i = 1; i <= 5; ++i) {
        Room* r = RoomFactory::createRoom(
            RoomFactory::Category::Econom,
            i,             // id
            100 + i,       // номер: 101, 102, 103, 104, 105
            2000,          // цена
            1              // вместимость
            );
        if (r) {
            r->addFeature(wiFi);
            r->addFeature(tv);
            Hotel::instance().addRoom(r);
        }
    }

    // ============================================================
    // 5. 3 номера "Стандарт" (201–203)
    // ============================================================
    for (int i = 1; i <= 3; ++i) {
        Room* r = RoomFactory::createRoom(
            RoomFactory::Category::Standard,
            10 + i,        // id
            200 + i,       // номер: 201, 202, 203
            3500,
            2
            );
        if (r) {
            r->addFeature(wiFi);
            r->addFeature(tv);
            r->addFeature(conditioner);
            Hotel::instance().addRoom(r);
        }
    }

    // ============================================================
    // 6. 1 номер "Люкс" (301)
    // ============================================================
    Room* lux = RoomFactory::createRoom(
        RoomFactory::Category::Lux,
        20,      // id
        301,     // номер
        8000,
        3
        );
    if (lux) {
        lux->addFeature(wiFi);
        lux->addFeature(tv);
        lux->addFeature(conditioner);
        lux->addFeature(minibar);
        lux->addFeature(jacuzzi);
        Hotel::instance().addRoom(lux);
    }

    // ============================================================
    // 7. Проверка в консоли
    // ============================================================
    qDebug() << "=== База инициализирована ===";
    qDebug() << "Номеров:   " << Hotel::instance().getRooms().size();
    qDebug() << "Удобств:   " << Hotel::instance().getFeatures().size();
    qDebug() << "Пожеланий: " << Hotel::instance().getPreferences().size();
}