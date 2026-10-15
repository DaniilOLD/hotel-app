#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QCloseEvent>
#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onShowRooms();
    void onShowGuests();
    void onShowBookings();
    void onAddGuest();
    void onDeleteGuest();
    void onSelectRoom();
    void onDeleteBooking();
    void onFreeRooms();
    void onCurrentGuests();
    void onStatistics();
    void onAbout();
private:
    Ui::MainWindow *ui;

    // Модели
    QStandardItemModel *m_roomsModel;
    QStandardItemModel *m_guestsModel;
    QStandardItemModel *m_bookingsModel;

    // Вспомогательные методы
    void setupModels();
    void fillRoomsTable();
    void fillGuestsTable();
    void fillBookingsTable();
    void setupReportsModel();
protected:
    void closeEvent(QCloseEvent* event) override;
};

#endif // MAINWINDOW_H