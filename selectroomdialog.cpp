#include "selectroomdialog.h"
#include "ui_selectroomdialog.h"

#include "hotel.h"
#include "selectionstrategy.h"
#include "booking.h"

#include <QDebug>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDate>

// ============================================================
// Конструктор
// ============================================================
SelectRoomDialog::SelectRoomDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectRoomDialog)
    , m_resultsModel(nullptr)
{
    ui->setupUi(this);

    // --- Модель для таблицы ---
    m_resultsModel = new QStandardItemModel(this);
    m_resultsModel->setHorizontalHeaderLabels({
        "ID", "Номер", "Категория", "Цена", "Вместимость", "Удобства"
    });
    ui->resultsTable->setModel(m_resultsModel);
    ui->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->resultsTable->horizontalHeader()->setStretchLastSection(true);
    ui->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // --- Заполняем список гостей ---
    fillGuests();

    // --- Вместимость: 1–3 ---
    ui->capacitySpin->setMinimum(1);
    ui->capacitySpin->setMaximum(3);
    ui->capacitySpin->setValue(1);

    // --- Даты по умолчанию ---
    ui->checkInEdit->setDate(QDate::currentDate());
    ui->checkOutEdit->setDate(QDate::currentDate().addDays(1));

    // --- Подключаем кнопки ---
    connect(ui->selectButton, &QPushButton::clicked,
            this, &SelectRoomDialog::onSelectClicked);
    connect(ui->bookButton, &QPushButton::clicked,
            this, &SelectRoomDialog::onBookClicked);

    connect(ui->closeButton, &QPushButton::clicked,
            this, &QDialog::reject);
}

SelectRoomDialog::~SelectRoomDialog()
{
    delete ui;
}

// ============================================================
// fillGuests() — список гостей
// ============================================================
void SelectRoomDialog::fillGuests()
{
    ui->guestCombo->clear();

    for (Guest* g : Hotel::instance().getGuests()) {
        ui->guestCombo->addItem(
            QString("%1 (%2)").arg(g->getFullName()).arg(g->getPhone()),
            QVariant::fromValue(static_cast<void*>(g))
            );
    }

    if (ui->guestCombo->count() == 0) {
        ui->guestCombo->addItem("— Нет гостей —");
        ui->selectButton->setEnabled(false);
    }
}

// ============================================================
// fillResults() — таблица результатов
// ============================================================
void SelectRoomDialog::fillResults(const std::vector<Room*>& rooms)
{
    m_resultsModel->removeRows(0, m_resultsModel->rowCount());

    for (Room* room : rooms) {
        QString features;
        for (Feature* f : room->getFeatures()) {
            if (!features.isEmpty()) features += ", ";
            features += f->getName();
        }

        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(room->getId()));
        row << new QStandardItem(QString::number(room->getNumber()));
        row << new QStandardItem(room->getCategory());
        row << new QStandardItem(QString::number(room->getPrice()));
        row << new QStandardItem(QString::number(room->getCapacity()));
        row << new QStandardItem(features);

        m_resultsModel->appendRow(row);
    }

    qDebug() << "Подобрано номеров:" << rooms.size();
}

// ============================================================
// getGuestPrefsString() — пожелания в строку
// ============================================================
QString SelectRoomDialog::getGuestPrefsString(Guest* guest)
{
    if (!guest) return "";
    QString s;
    for (Preference* p : guest->getPreferences())
        s += "• " + p->getName() + "\n";
    if (s.isEmpty()) return "— нет пожеланий —";
    return s;
}

// ============================================================
// onSelectClicked() — кнопка «Подобрать»
// ============================================================
void SelectRoomDialog::onSelectClicked()
{
    void* ptr = ui->guestCombo->currentData().value<void*>();
    Guest* guest = static_cast<Guest*>(ptr);

    if (!guest) {
        QMessageBox::warning(this, "Ошибка", "Выберите гостя!");
        return;
    }

    int capacity = ui->capacitySpin->value();
    std::vector<Room*> allRooms;

    for (Room* r : Hotel::instance().getRooms()) {
        if (r->getStatus() != RoomStatus::Free) continue;
        if (r->getCapacity() < capacity) continue;
        allRooms.push_back(r);
    }

    if (allRooms.empty()) {
        QMessageBox::information(this, "Результат",
                                 QString("Нет свободных номеров вместимостью %1+ мест!").arg(capacity));
        m_resultsModel->removeRows(0, m_resultsModel->rowCount());
        return;
    }

    ByPreferencesStrategy strategy;
    std::vector<Room*> result = strategy.select(guest, allRooms);

    fillResults(result);

    if (result.empty()) {
        QMessageBox::information(this, "Результат",
                                 "По пожеланиям гостя ничего не найдено.\n\n"
                                 "Пожелания гостя:\n" + getGuestPrefsString(guest));
    }
}

// ============================================================
// onBookClicked() — кнопка «Забронировать»
// ============================================================
void SelectRoomDialog::onBookClicked()
{
    // 1. Проверка: выбран ли гость
    void* guestPtr = ui->guestCombo->currentData().value<void*>();
    Guest* guest = static_cast<Guest*>(guestPtr);
    if (!guest) {
        QMessageBox::warning(this, "Ошибка", "Выберите гостя!");
        return;
    }

    // 2. Проверка: выбрана ли строка в таблице
    QModelIndexList selected = ui->resultsTable->selectionModel()
                                   ->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Выберите номер из таблицы!");
        return;
    }

    // 3. Получаем номер по ID из первой колонки
    int row = selected.first().row();
    int roomId = m_resultsModel->item(row, 0)->text().toInt();

    Room* room = nullptr;
    for (Room* r : Hotel::instance().getRooms())
        if (r->getId() == roomId) { room = r; break; }

    if (!room) {
        QMessageBox::warning(this, "Ошибка", "Номер не найден!");
        return;
    }

    // 4. Проверяем даты
    QDate checkIn  = ui->checkInEdit->date();
    QDate checkOut = ui->checkOutEdit->date();

    if (checkIn >= checkOut) {
        QMessageBox::warning(this, "Ошибка",
                             "Дата выезда должна быть позже даты заезда!");
        return;
    }

    if (checkIn < QDate::currentDate()) {
        QMessageBox::warning(this, "Ошибка",
                             "Дата заезда не может быть в прошлом!");
        return;
    }

    // 5. Создаём бронь
    int newId = Hotel::instance().getBookings().size() + 1;
    Booking* booking = new Booking(newId, guest, room, checkIn, checkOut);
    Hotel::instance().addBooking(booking);

    // 6. Меняем статус номера на «Занят»
    room->setStatus(RoomStatus::Occupied);

    // 7. Уведомление
    QMessageBox::information(this, "Успех",
                             QString("Номер %1 забронирован!\n\n"
                                     "Гость: %2\n"
                                     "Заезд: %3\n"
                                     "Выезд: %4\n"
                                     "Ночей: %5")
                                 .arg(room->getNumber())
                                 .arg(guest->getFullName())
                                 .arg(checkIn.toString("dd.MM.yyyy"))
                                 .arg(checkOut.toString("dd.MM.yyyy"))
                                 .arg(booking->getNights()));

    qDebug() << "Создана бронь:"
             << guest->getFullName()
             << "→ номер" << room->getNumber()
             << "(" << booking->getNights() << "ночей)";

    // 8. Обновляем таблицу (номер стал занят)
    accept();
}