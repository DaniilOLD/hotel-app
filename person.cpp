#include "person.h"

Person::Person() : m_fullName(""), m_phone("") {}

Person::Person(const QString& fullName, const QString& phone)
    : m_fullName(fullName), m_phone(phone) {}

QString Person::getFullName() const { return m_fullName; }
QString Person::getPhone() const { return m_phone; }

void Person::setFullName(const QString& fullName) { m_fullName = fullName; }
void Person::setPhone(const QString& phone) { m_phone = phone; }
