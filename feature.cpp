#include "feature.h"

Feature::Feature() : m_id(0), m_name("") {}

Feature::Feature(int id, const QString& name)
    : m_id(id), m_name(name) {}

int Feature::getId() const { return m_id; }
QString Feature::getName() const { return m_name; }

void Feature::setId(int id) { m_id = id; }
void Feature::setName(const QString& name) { m_name = name; }