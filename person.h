#ifndef PERSON_H
#define PERSON_H

#include <QString>

// Абстрактный базовый класс — лицо
class Person
{
public:
    Person();
    Person(const QString& fullName, const QString& phone);
    virtual ~Person() = default;

    QString getFullName() const;
    QString getPhone() const;

    void setFullName(const QString& fullName);
    void setPhone(const QString& phone);

    // чисто виртуальный — переопределяется в Guest
    virtual QString getInfo() const = 0;

protected:
    QString m_fullName;
    QString m_phone;
};

#endif // PERSON_H
