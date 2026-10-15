#include "mainwindow.h"
#include <QApplication>
#include "filemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileManager::load(FileManager::defaultFilename());
    MainWindow w;
    w.show();
    return QApplication::exec();
}