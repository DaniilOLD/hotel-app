#include "preference.h"

Preference::Preference() : m_id(0), m_name("") {}

Preference::Preference(int id, const QString& name)
    : m_id(id), m_name(name) {}

int Preference::getId() const { return m_id; }
QString Preference::getName() const { return m_name; }

void Preference::setId(int id) { m_id = id; }
void Preference::setName(const QString& name) { m_name = name; }
