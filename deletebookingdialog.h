#ifndef DELETEBOOKINGDIALOG_H
#define DELETEBOOKINGDIALOG_H

#include <QDialog>
#include "booking.h"

namespace Ui {
class DeleteBookingDialog;
}

class DeleteBookingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteBookingDialog(QWidget *parent = nullptr);
    ~DeleteBookingDialog() override;

    Booking* getSelectedBooking() const { return m_selected; }

private slots:
    void onAccepted();

private:
    Ui::DeleteBookingDialog *ui;
    Booking* m_selected;
};

#endif // DELETEBOOKINGDIALOG_H