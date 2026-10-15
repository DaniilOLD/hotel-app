#ifndef SELECTROOMDIALOG_H
#define SELECTROOMDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "guest.h"
#include "room.h"

namespace Ui {
class SelectRoomDialog;
}

class SelectRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectRoomDialog(QWidget *parent = nullptr);
    ~SelectRoomDialog() override;

private slots:
    void onSelectClicked();   // «Подобрать»
    void onBookClicked();     // «Забронировать»

private:
    Ui::SelectRoomDialog *ui;
    QStandardItemModel *m_resultsModel;

    void fillGuests();
    void fillResults(const std::vector<Room*>& rooms);
    QString getGuestPrefsString(Guest* guest);
};

#endif // SELECTROOMDIALOG_H