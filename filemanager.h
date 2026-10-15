#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>

// ============================================================
// FileManager — сохранение и загрузка данных
// ============================================================
class FileManager
{
public:
    // Сохранить всё в файл
    static bool save(const QString& filename);

    // Загрузить всё из файла
    static bool load(const QString& filename);

    // Путь к файлу по умолчанию
    static QString defaultFilename();
};

#endif // FILEMANAGER_H