#ifndef FEATURE_H
#define FEATURE_H
#include <QString>

// Удобство номера (Wi-Fi, ТВ, кондиционер...)
class Feature
{
public:
    Feature();
    Feature(int id, const QString& name);

    int getId() const;
    QString getName() const;

    void setId(int id);
    void setName(const QString& name);

private:
    int m_id;
    QString m_name;
};
#endif // FEATURE_H
