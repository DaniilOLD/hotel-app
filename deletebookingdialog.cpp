#include "deletebookingdialog.h"
#include "ui_deletebookingdialog.h"

#include "hotel.h"

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>

// ============================================================
// Конструктор
// ============================================================
DeleteBookingDialog::DeleteBookingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteBookingDialog)
    , m_selected(nullptr)
{
    ui->setupUi(this);

    // Заполняем список броней
    const auto& bookings = Hotel::instance().getBookings();

    if (bookings.empty()) {
        ui->bookingCombo->addItem("— Нет броней —");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        for (Booking* b : bookings) {
            if (!b->getGuest() || !b->getRoom()) continue;

            ui->bookingCombo->addItem(
                QString("%1 → №%2 (%3 - %4)")
                    .arg(b->getGuest()->getFullName())
                    .arg(b->getRoom()->getNumber())
                    .arg(b->getCheckIn().toString("dd.MM.yyyy"))
                    .arg(b->getCheckOut().toString("dd.MM.yyyy")),
                QVariant::fromValue(static_cast<void*>(b))
                );
        }
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DeleteBookingDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
}

DeleteBookingDialog::~DeleteBookingDialog()
{
    delete ui;
}

// ============================================================
// onAccepted() — при нажатии OK
// ============================================================
void DeleteBookingDialog::onAccepted()
{
    void* ptr = ui->bookingCombo->currentData().value<void*>();
    m_selected = static_cast<Booking*>(ptr);

    if (!m_selected) {
        QMessageBox::warning(this, "Ошибка",
                             "Выберите бронь для удаления!");
        return;
    }

    // Подтверждение
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Подтверждение удаления",
        QString("Удалить бронь:\n\n"
                "Гость: %1\n"
                "Номер: %2\n"
                "Заезд: %3\n"
                "Выезд: %4\n\n"
                "Номер станет свободным. Продолжить?")
            .arg(m_selected->getGuest()->getFullName())
            .arg(m_selected->getRoom()->getNumber())
            .arg(m_selected->getCheckIn().toString("dd.MM.yyyy"))
            .arg(m_selected->getCheckOut().toString("dd.MM.yyyy")),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (reply != QMessageBox::Yes) return;

    accept();
}