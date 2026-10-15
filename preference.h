#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QString>

// Пожелание гостя (тихий этаж, вид на море...)
class Preference
{
public:
    Preference();
    Preference(int id, const QString& name);

    int getId() const;
    QString getName() const;

    void setId(int id);
    void setName(const QString& name);

private:
    int m_id;
    QString m_name;
};


#endif // PREFERENCE_H
