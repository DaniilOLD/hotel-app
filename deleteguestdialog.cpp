#include "deleteguestdialog.h"
#include "ui_deleteguestdialog.h"

#include "hotel.h"

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>

// ============================================================
// Конструктор
// ============================================================
DeleteGuestDialog::DeleteGuestDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteGuestDialog)
    , m_selected(nullptr)
{
    ui->setupUi(this);

    // Заполняем список гостей
    const auto& guests = Hotel::instance().getGuests();

    if (guests.empty()) {
        ui->guestCombo->addItem("— Нет гостей —");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        for (Guest* g : guests) {
            ui->guestCombo->addItem(
                QString("%1 (%2)")
                    .arg(g->getFullName())
                    .arg(g->getPhone()),
                QVariant::fromValue(static_cast<void*>(g))
                );
        }
    }

    // Подключаем кнопки
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DeleteGuestDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
}

DeleteGuestDialog::~DeleteGuestDialog()
{
    delete ui;
}

// ============================================================
// onAccepted() — при нажатии OK
// ============================================================
void DeleteGuestDialog::onAccepted()
{
    // Получаем выбранного гостя
    void* ptr = ui->guestCombo->currentData().value<void*>();
    m_selected = static_cast<Guest*>(ptr);

    if (!m_selected) {
        QMessageBox::warning(this, "Ошибка",
                             "Выберите гостя для удаления!");
        return;
    }

    // Подтверждение
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Подтверждение удаления",
        QString("Удалить гостя:\n\n"
                "ФИО: %1\n"
                "Телефон: %2\n\n"
                "Вы уверены?")
            .arg(m_selected->getFullName())
            .arg(m_selected->getPhone()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (reply != QMessageBox::Yes) {
        return;
    }

    accept();
}