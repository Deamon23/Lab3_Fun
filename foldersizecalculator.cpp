#include "FolderSizeCalculator.h"

QMap<QString, qint64> FolderSizeCalculator::calculateSize(const QString &path, qint64 &totalSize) {
    QMap<QString, qint64> folderSizes;
    qint64 currentDirectorySize = 0;
    QDir dir(path);

    // Проверка существования директории
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << path;
        return folderSizes;
    }

    // Получение списка подкаталогов верхнего уровня
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    // Обход каждого подкаталога
    foreach (const QFileInfo &folderInfo, folderList) {
        qint64 folderTotalSize = 0;
        QDir folderDir(folderInfo.absoluteFilePath());

        // Получение списка файлов в текущем подкаталоге
        QFileInfoList subFileList = folderDir.entryInfoList(QDir::Files | QDir::NoSymLinks);

        // Суммирование размеров всех файлов в текущем подкаталоге
        foreach (const QFileInfo &fileInfo, subFileList) {
            folderTotalSize += fileInfo.size();
        }

        // Сохранение общего размера файлов в подкаталоге
        folderSizes[folderInfo.fileName()] = folderTotalSize;
        totalSize += folderTotalSize;
    }

    // Суммирование размеров всех файлов верхнего уровня, которые не являются папками
    foreach (const QFileInfo &fileInfo, fileList) {
        currentDirectorySize += fileInfo.size();
    }

    // Добавление не папковых файлов в результат
    folderSizes["CurrentDirectory"] = currentDirectorySize;
    totalSize += currentDirectorySize;
    folderSizes["other"] = 0;
    return folderSizes;
}
