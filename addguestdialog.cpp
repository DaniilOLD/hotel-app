#include "addguestdialog.h"
#include "ui_addguestdialog.h"

#include "hotel.h"
#include "preference.h"

#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

// ============================================================
// Конструктор
// ============================================================
AddGuestDialog::AddGuestDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddGuestDialog)
    , m_guest(nullptr)
{
    ui->setupUi(this);

    // Валидатор для телефона — только цифры, +, -, пробел, скобки
    QRegularExpression phoneRe("^[0-9+\\-\\s()]*$");
    ui->phoneEdit->setValidator(new QRegularExpressionValidator(phoneRe, this));

    // Валидатор для ФИО — буквы, пробел, дефис (без цифр)
    QRegularExpression nameRe("^[A-Za-zА-Яа-яЁё\\s\\-]*$");
    ui->nameEdit->setValidator(new QRegularExpressionValidator(nameRe, this));

    // Подключаем кнопки
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &AddGuestDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
}

AddGuestDialog::~AddGuestDialog()
{
    delete ui;
}

// ============================================================
// onAccepted() — при нажатии OK
// ============================================================
void AddGuestDialog::onAccepted()
{
    // ============================================================
    // ПРОВЕРКА ФИО
    // ============================================================
    QString fullName = ui->nameEdit->text().trimmed();

    // 1. Не пустое
    if (fullName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ФИО гостя!");
        ui->nameEdit->setFocus();
        return;
    }

    // 2. Минимум 3 символа
    if (fullName.length() < 3) {
        QMessageBox::warning(this, "Ошибка",
                             "ФИО слишком короткое (минимум 3 символа)!");
        ui->nameEdit->setFocus();
        return;
    }

    // ============================================================
    // ПРОВЕРКА ТЕЛЕФОНА
    // ============================================================
    QString phone = ui->phoneEdit->text().trimmed();

    // 1. Не пустой
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите телефон гостя!");
        ui->phoneEdit->setFocus();
        return;
    }

    // 2. Только допустимые символы
    QRegularExpression re("^[0-9+\\-\\s()]+$");
    if (!re.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Ошибка",
                             "Телефон должен содержать только цифры!\n"
                             "Допускаются также символы: +, -, пробел, скобки.");
        ui->phoneEdit->setFocus();
        return;
    }

    // 3. Считаем только цифры
    QString digitsOnly;
    for (QChar c : phone)
        if (c.isDigit()) digitsOnly += c;

    // 4. Длина: 10 или 11 цифр
    if (digitsOnly.length() < 10 || digitsOnly.length() > 11) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Телефон должен содержать 10 или 11 цифр!\n"
                                     "Введено цифр: %1")
                                 .arg(digitsOnly.length()));
        ui->phoneEdit->setFocus();
        return;
    }

    // 5. Проверка уникальности — телефон не должен повторяться
    for (Guest* g : Hotel::instance().getGuests()) {
        if (g->getPhone() == phone) {
            QMessageBox::warning(this, "Ошибка",
                                 "Гость с таким телефоном уже существует!");
            ui->phoneEdit->setFocus();
            return;
        }
    }

    // ============================================================
    // СОЗДАНИЕ ГОСТЯ
    // ============================================================
    int newId = Hotel::instance().getGuests().size() + 1;
    m_guest = new Guest(newId, fullName, phone);

    // ============================================================
    // ПОЖЕЛАНИЯ (МНОГОЗНАЧНАЯ ЗАВИСИМОСТЬ)
    // ============================================================
    const auto& allPrefs = Hotel::instance().getPreferences();

    auto addPref = [&](bool checked, const QString& name) {
        if (!checked) return;
        for (Preference* p : allPrefs)
            if (p->getName() == name) {
                m_guest->addPreference(p);
                break;
            }
    };

    addPref(ui->prefWiFi->isChecked(),        "Wi-Fi");
    addPref(ui->prefTV->isChecked(),          "Телевизор");
    addPref(ui->prefConditioner->isChecked(), "Кондиционер");
    addPref(ui->prefMinibar->isChecked(),     "Мини-бар");
    addPref(ui->prefJacuzzi->isChecked(),     "Джакузи");

    qDebug() << "Создан гость:" << m_guest->getInfo();

    // ============================================================
    // ЗАКРЫТИЕ ДИАЛОГА
    // ============================================================
    accept();
}