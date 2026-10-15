#include "guest.h"
#include <algorithm>

Guest::Guest() : Person(), m_id(0) {}

Guest::Guest(int id, const QString& fullName, const QString& phone)
    : Person(fullName, phone), m_id(id) {}

int Guest::getId() const { return m_id; }
void Guest::setId(int id) { m_id = id; }

// --- МНОГОЗНАЧНАЯ ЗАВИСИМОСТЬ ---
void Guest::addPreference(Preference* pref) {
    if (pref) m_preferences.push_back(pref);
}

void Guest::removePreference(int preferenceId) {
    m_preferences.erase(
        std::remove_if(m_preferences.begin(), m_preferences.end(),
                       [preferenceId](Preference* p) { return p->getId() == preferenceId; }),
        m_preferences.end());
}

const std::vector<Preference*>& Guest::getPreferences() const {
    return m_preferences;
}

QString Guest::getInfo() const {
    QString info = m_fullName + " (" + m_phone + ")";
    if (!m_preferences.empty()) {
        info += " | пожелания: ";
        for (auto* p : m_preferences) info += p->getName() + " ";
    }
    return info;
}