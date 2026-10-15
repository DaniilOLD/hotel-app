#ifndef GUEST_H
#define GUEST_H

#include "person.h"
#include "preference.h"
#include <vector>

// Гость — наследник Person + МЗ (пожелания)
class Guest : public Person
{
public:
    Guest();
    Guest(int id, const QString& fullName, const QString& phone);

    int getId() const;
    void setId(int id);

    // --- МНОГОЗНАЧНАЯ ЗАВИСИМОСТЬ: у гостя много пожеланий ---
    void addPreference(Preference* pref);
    void removePreference(int preferenceId);
    const std::vector<Preference*>& getPreferences() const;

    QString getInfo() const override;

private:
    int m_id;
    std::vector<Preference*> m_preferences;   // ← МЗ
};


#endif // GUEST_H
