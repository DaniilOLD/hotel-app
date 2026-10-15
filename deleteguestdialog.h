#ifndef DELETEGUESTDIALOG_H
#define DELETEGUESTDIALOG_H

#include <QDialog>
#include "guest.h"

namespace Ui {
class DeleteGuestDialog;
}

class DeleteGuestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteGuestDialog(QWidget *parent = nullptr);
    ~DeleteGuestDialog() override;

    Guest* getSelectedGuest() const { return m_selected; }

private slots:
    void onAccepted();

private:
    Ui::DeleteGuestDialog *ui;
    Guest* m_selected;
};

#endif // DELETEGUESTDIALOG_H