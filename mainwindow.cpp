#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initializer.h"

// Классы ядра
#include "hotel.h"
#include "room.h"
#include "guest.h"
#include "preference.h"

// Форма добавления гостя
#include "addguestdialog.h"
#include "deleteguestdialog.h"
#include "booking.h"
#include "selectroomdialog.h"
#include "deletebookingdialog.h"
#include <QMessageBox>
#include <QDebug>
#include "filemanager.h"
// ============================================================
// Конструктор
// ============================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_roomsModel(nullptr)
    , m_guestsModel(nullptr)
    , m_bookingsModel(nullptr)
{
    ui->setupUi(this);

    initData();      // заполняем базу
    setupModels();   // настраиваем модели и таблицы

    // Подключаем пункты меню к слотам
    connect(ui->actionShowRooms,    &QAction::triggered, this, &MainWindow::onShowRooms);
    connect(ui->actionShowGuests,   &QAction::triggered, this, &MainWindow::onShowGuests);
    connect(ui->actionShowBookings, &QAction::triggered, this, &MainWindow::onShowBookings);
    connect(ui->actionAddGuest,     &QAction::triggered, this, &MainWindow::onAddGuest);
    connect(ui->actionDeleteGuest,  &QAction::triggered, this, &MainWindow::onDeleteGuest);
    connect(ui->actionSelectRoom, &QAction::triggered, this, &MainWindow::onSelectRoom);
    connect(ui->actionDeleteBooking, &QAction::triggered, this, &MainWindow::onDeleteBooking);
    connect(ui->actionFreeRooms,     &QAction::triggered, this, &MainWindow::onFreeRooms);
    connect(ui->actionCurrentGuests, &QAction::triggered, this, &MainWindow::onCurrentGuests);
    connect(ui->actionStatistics,    &QAction::triggered, this, &MainWindow::onStatistics);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================================
// setupModels() — создаём модели и привязываем к таблицам
// ============================================================
void MainWindow::setupModels()
{
    // --- Номера ---
    m_roomsModel = new QStandardItemModel(this);
    m_roomsModel->setHorizontalHeaderLabels({
        "ID", "Номер", "Категория", "Цена", "Вместимость", "Статус", "Удобства"
    });
    ui->roomsTable->setModel(m_roomsModel);
    ui->roomsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->roomsTable->horizontalHeader()->setStretchLastSection(true);

    // --- Гости ---
    m_guestsModel = new QStandardItemModel(this);
    m_guestsModel->setHorizontalHeaderLabels({
        "ID", "ФИО", "Телефон", "Пожелания", "Статус"
    });
    ui->guestsTable->setModel(m_guestsModel);
    ui->guestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->guestsTable->horizontalHeader()->setStretchLastSection(true);

    // --- Брони ---
    m_bookingsModel = new QStandardItemModel(this);
    m_bookingsModel->setHorizontalHeaderLabels({
        "ID", "Гость", "Номер", "Заезд", "Выезд", "Ночей"
    });
    ui->bookingsTable->setModel(m_bookingsModel);
    ui->bookingsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->bookingsTable->horizontalHeader()->setStretchLastSection(true);


    // Заполняем таблицы
    fillRoomsTable();
    fillGuestsTable();
    fillBookingsTable();

    // Настраиваем таблицу отчётов (пока пустая)
    ui->reportsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->reportsTable->horizontalHeader()->setStretchLastSection(true);
}
void MainWindow::onSelectRoom()
{
    SelectRoomDialog dialog(this);
    dialog.exec();
}
// ============================================================
// onDeleteGuest() — удалить гостя
// ============================================================
void MainWindow::onDeleteGuest()
{
    DeleteGuestDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Guest* guest = dialog.getSelectedGuest();
        if (!guest) return;

        QString name = guest->getFullName();
        int guestId = guest->getId();

        // Проверка: есть ли у гостя брони
        bool hasBookings = false;
        for (Booking* b : Hotel::instance().getBookings()) {
            if (b->getGuest() && b->getGuest()->getId() == guestId) {
                hasBookings = true;
                break;
            }
        }

        if (hasBookings) {
            QMessageBox::warning(this, "Ошибка",
                                 QString("Нельзя удалить гостя %1 —\n"
                                         "у него есть активные брони!")
                                     .arg(name));
            return;
        }

        // Удаляем из Hotel
        Hotel::instance().removeGuest(guestId);

        // Освобождаем память
        delete guest;

        qDebug() << "Удалён гость:" << name;

        // Обновляем таблицу
        fillGuestsTable();
        ui->stackedWidget->setCurrentWidget(ui->pageGuests);
    }
}
// ============================================================
// fillRoomsTable() — таблица номеров
// ============================================================
void MainWindow::fillRoomsTable()
{
    m_roomsModel->removeRows(0, m_roomsModel->rowCount());

    for (Room* room : Hotel::instance().getRooms()) {
        QString features;
        for (Feature* f : room->getFeatures()) {
            if (!features.isEmpty()) features += ", ";
            features += f->getName();
        }

        QString status;
        switch (room->getStatus()) {
        case RoomStatus::Free:     status = "Свободен"; break;
        case RoomStatus::Occupied: status = "Занят";    break;
        }

        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(room->getId()));
        row << new QStandardItem(QString::number(room->getNumber()));
        row << new QStandardItem(room->getCategory());
        row << new QStandardItem(QString::number(room->getPrice()));
        row << new QStandardItem(QString::number(room->getCapacity()));
        row << new QStandardItem(status);
        row << new QStandardItem(features);

        m_roomsModel->appendRow(row);
    }
}

// ============================================================
// fillGuestsTable() — таблица гостей
// ============================================================
// ============================================================
// fillGuestsTable() — таблица гостей (с колонкой «Статус»)
// ============================================================
void MainWindow::fillGuestsTable()
{
    m_guestsModel->removeRows(0, m_guestsModel->rowCount());

    for (Guest* g : Hotel::instance().getGuests()) {
        // --- Пожелания ---
        QString prefs;
        for (Preference* p : g->getPreferences()) {
            if (!prefs.isEmpty()) prefs += ", ";
            prefs += p->getName();
        }

        // --- Статус: заселён? ---
        QString status = "Не заселён";

        for (Booking* b : Hotel::instance().getBookings()) {
            if (b->getGuest() == g &&
                b->getRoom() &&
                b->getRoom()->getStatus() == RoomStatus::Occupied)
            {
                status = QString("Заселён (№%1)")
                             .arg(b->getRoom()->getNumber());
                break;
            }
        }

        // --- Строка таблицы ---
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(g->getId()));
        row << new QStandardItem(g->getFullName());
        row << new QStandardItem(g->getPhone());
        row << new QStandardItem(prefs.isEmpty() ? "—" : prefs);
        row << new QStandardItem(status);   // ← НОВАЯ колонка

        m_guestsModel->appendRow(row);
    }
}
// ============================================================
// fillBookingsTable() — заполняем таблицу броней
// ============================================================
void MainWindow::fillBookingsTable()
{
    m_bookingsModel->removeRows(0, m_bookingsModel->rowCount());

    for (Booking* b : Hotel::instance().getBookings()) {
        if (!b->getGuest() || !b->getRoom()) continue;

        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(b->getId()));
        row << new QStandardItem(b->getGuest()->getFullName());
        row << new QStandardItem(QString::number(b->getRoom()->getNumber()));
        row << new QStandardItem(b->getCheckIn().toString("dd.MM.yyyy"));
        row << new QStandardItem(b->getCheckOut().toString("dd.MM.yyyy"));
        row << new QStandardItem(QString::number(b->getNights()));

        m_bookingsModel->appendRow(row);
    }

    qDebug() << "Заполнено броней:" << m_bookingsModel->rowCount();
}

// ============================================================
// Слоты — обработчики меню
// ============================================================

void MainWindow::onShowRooms()
{
    fillRoomsTable();
    ui->stackedWidget->setCurrentWidget(ui->pageRooms);
    qDebug() << "Показаны номера, всего:" << m_roomsModel->rowCount();
}

void MainWindow::onShowGuests()
{
    fillGuestsTable();
    ui->stackedWidget->setCurrentWidget(ui->pageGuests);
    qDebug() << "Показаны гости, всего:" << m_guestsModel->rowCount();
}

void MainWindow::onShowBookings()
{
    fillBookingsTable();
    ui->stackedWidget->setCurrentWidget(ui->pageBookings);
    qDebug() << "Показаны брони, всего:" << m_bookingsModel->rowCount();
}

// ============================================================
// onAddGuest() — открыть диалог добавления гостя
// ============================================================
void MainWindow::onAddGuest()
{
    AddGuestDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Guest* guest = dialog.getGuest();
        if (guest) {
            Hotel::instance().addGuest(guest);
            qDebug() << "Добавлен гость:" << guest->getFullName();

            // Обновляем таблицу и переключаемся на страницу гостей
            fillGuestsTable();
            ui->stackedWidget->setCurrentWidget(ui->pageGuests);
        }
    }
}
// ============================================================
// onDeleteBooking() — удалить бронь (выселить + удалить гостя)
// ============================================================
void MainWindow::onDeleteBooking()
{
    DeleteBookingDialog dialog(this);

    if (dialog.exec() != QDialog::Accepted) return;

    Booking* booking = dialog.getSelectedBooking();
    if (!booking) return;

    Room* room = booking->getRoom();
    Guest* guest = booking->getGuest();

    QString guestName = guest ? guest->getFullName() : "—";
    int roomNumber = room ? room->getNumber() : 0;

    // 1. Освобождаем номер
    if (room) room->setStatus(RoomStatus::Free);

    // 2. Удаляем бронь
    Hotel::instance().removeBooking(booking->getId());
    delete booking;

    // 3. Удаляем гостя из базы (АВТОМАТИЧЕСКИ)
    if (guest) {
        Hotel::instance().removeGuest(guest->getId());
        delete guest;
    }

    qDebug() << "Удалена бронь и гость:" << guestName << "→ номер" << roomNumber;

    // 4. Сообщение о выселении
    QMessageBox::information(
        this,
        "Гость выселен",
        QString("Гость %1 выселен из номера %2.\n\n"
                "Хорошего дня!")
            .arg(guestName)
            .arg(roomNumber)
        );

    // 5. Обновляем все таблицы
    fillBookingsTable();
    fillRoomsTable();
    fillGuestsTable();

    ui->stackedWidget->setCurrentWidget(ui->pageBookings);
}
// ============================================================
// onFreeRooms() — отчёт «Свободные номера»
// ============================================================
void MainWindow::onFreeRooms()
{
    // --- Модель для отчёта ---
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "ID", "Номер", "Категория", "Цена", "Вместимость", "Удобства"
    });

    // --- Заполняем только свободные ---
    for (Room* r : Hotel::instance().getRooms()) {
        if (r->getStatus() != RoomStatus::Free) continue;

        QString features;
        for (Feature* f : r->getFeatures()) {
            if (!features.isEmpty()) features += ", ";
            features += f->getName();
        }

        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(r->getId()));
        row << new QStandardItem(QString::number(r->getNumber()));
        row << new QStandardItem(r->getCategory());
        row << new QStandardItem(QString::number(r->getPrice()));
        row << new QStandardItem(QString::number(r->getCapacity()));
        row << new QStandardItem(features);

        model->appendRow(row);
    }

    // --- Применяем модель ---
    ui->reportsTable->setModel(model);

    // --- Переключаем на страницу отчётов ---
    ui->stackedWidget->setCurrentWidget(ui->pageReports);

    qDebug() << "Отчёт: свободных номеров —" << model->rowCount();
}

// ============================================================
// onCurrentGuests() — отчёт «Проживающие гости»
// ============================================================
void MainWindow::onCurrentGuests()
{
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "ID", "ФИО", "Телефон", "Номер", "Заезд", "Выезд", "Ночей"
    });

    for (Booking* b : Hotel::instance().getBookings()) {
        if (!b->getGuest() || !b->getRoom()) continue;

        // Проверяем, что номер занят
        if (b->getRoom()->getStatus() != RoomStatus::Occupied) continue;

        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(b->getGuest()->getId()));
        row << new QStandardItem(b->getGuest()->getFullName());
        row << new QStandardItem(b->getGuest()->getPhone());
        row << new QStandardItem(QString::number(b->getRoom()->getNumber()));
        row << new QStandardItem(b->getCheckIn().toString("dd.MM.yyyy"));
        row << new QStandardItem(b->getCheckOut().toString("dd.MM.yyyy"));
        row << new QStandardItem(QString::number(b->getNights()));

        model->appendRow(row);
    }

    ui->reportsTable->setModel(model);
    ui->stackedWidget->setCurrentWidget(ui->pageReports);

    qDebug() << "Отчёт: проживающих гостей —" << model->rowCount();
}

// ============================================================
// onStatistics() — отчёт «Статистика»
// ============================================================
void MainWindow::onStatistics()
{
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({ "Показатель", "Значение" });

    // --- Всего ---
    int totalRooms    = Hotel::instance().getRooms().size();
    int totalGuests   = Hotel::instance().getGuests().size();
    int totalBookings = Hotel::instance().getBookings().size();

    // --- По статусам ---
    int freeRooms = 0, occupiedRooms = 0;
    for (Room* r : Hotel::instance().getRooms()) {
        if (r->getStatus() == RoomStatus::Free)     freeRooms++;
        if (r->getStatus() == RoomStatus::Occupied) occupiedRooms++;
    }

    // --- По категориям ---
    int econom = 0, standard = 0, lux = 0;
    for (Room* r : Hotel::instance().getRooms()) {
        QString cat = r->getCategory();
        if (cat == "Эконом")   econom++;
        if (cat == "Стандарт") standard++;
        if (cat == "Люкс")     lux++;
    }

    // --- Заполняем ---
    auto addRow = [&](const QString& name, const QString& value) {
        QList<QStandardItem*> row;
        row << new QStandardItem(name);
        row << new QStandardItem(value);
        model->appendRow(row);
    };

    addRow("Всего номеров",       QString::number(totalRooms));
    addRow("Свободных номеров",   QString::number(freeRooms));
    addRow("Занятых номеров",     QString::number(occupiedRooms));
    addRow("",                    "");
    addRow("Категория «Эконом»",  QString::number(econom));
    addRow("Категория «Стандарт»",QString::number(standard));
    addRow("Категория «Люкс»",    QString::number(lux));
    addRow("",                    "");
    addRow("Всего гостей",        QString::number(totalGuests));
    addRow("Всего броней",        QString::number(totalBookings));

    ui->reportsTable->setModel(model);
    ui->stackedWidget->setCurrentWidget(ui->pageReports);

    qDebug() << "Отчёт: статистика";
}
// ============================================================
// onAbout() — диалог «О программе»
// ============================================================
void MainWindow::onAbout()
{
    QMessageBox::about(this,
                       "О программе",
                       "<h2>Гостиница</h2>"
                       "<p><b>Версия:</b> 1.0</p>"
                       "<p><b>Автор:</b> Ольшевский Даниил Денисович </p>"
                       "<p><b>Группа:</b> 4431Кз</p>"
                       "<hr>"
                       "<p>Программа для учёта и распределения гостей "
                       "по номерам гостиницы с учётом пожеланий.</p>"
                       "<p><b>Технологии:</b> C++, Qt 6, ООП, "
                       "паттерны проектирования.</p>"
                       "<p><b>Паттерны:</b> Singleton, Facade, "
                       "Factory Method, Strategy.</p>"
                       );
}
// ============================================================
// closeEvent() — автосохранение при выходе
// ============================================================
void MainWindow::closeEvent(QCloseEvent* event)
{
    // Спрашиваем пользователя
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Выход",
                                  "Сохранить данные перед выходом?",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        event->ignore();   // не закрываем окно
        return;
    }

    if (reply == QMessageBox::Yes) {
        if (FileManager::save(FileManager::defaultFilename())) {
            qDebug() << "Данные сохранены";
        } else {
            QMessageBox::warning(this, "Ошибка",
                                 "Не удалось сохранить данные!");
        }
    }

    event->accept();   // закрываем
}