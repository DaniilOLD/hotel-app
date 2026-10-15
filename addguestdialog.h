#ifndef ADDGUESTDIALOG_H
#define ADDGUESTDIALOG_H

#include <QDialog>
#include "guest.h"

namespace Ui {
class AddGuestDialog;
}

class AddGuestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddGuestDialog(QWidget *parent = nullptr);
    ~AddGuestDialog() override;

    Guest* getGuest() const { return m_guest; }

private slots:
    void onAccepted();

private:
    Ui::AddGuestDialog *ui;
    Guest* m_guest;
};

#endif // ADDGUESTDIALOG_H
