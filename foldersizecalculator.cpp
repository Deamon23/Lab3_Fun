#include "FolderSizeCalculator.h"

void calculateDirectorySize(const QString &path, qint64 &dirSize) {
    QDir dir(path);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    foreach (const QFileInfo &fileInfo, fileList) {
        if (fileInfo.isFile()) {
            dirSize += fileInfo.size();
        } else if (fileInfo.isDir()) {
            qint64 subDirSize = 0;
            calculateDirectorySize(fileInfo.absoluteFilePath(), subDirSize);
            dirSize += subDirSize;
        }
    }
}

QMap<QString, qint64> FolderSizeCalculator::calculateSize(const QString &path, qint64 &totalSize) {
    QMap<QString, qint64> folderSizes;
    QDir dir(path);

    // Проверка существования директории
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << path;
        return folderSizes;
    }

    // Получение списка всех файлов и папок в текущей директории
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    // Обход каждого элемента в директории
    foreach (const QFileInfo &fileInfo, fileList) {
        qint64 fileSize = 0;

        // Если элемент является файлом, учитываем его размер
        if (fileInfo.isFile()) {
            fileSize = fileInfo.size();
        }
        // Если элемент является директорией, учитываем ее размер
        else if (fileInfo.isDir()) {
            QString fileName = fileInfo.fileName();
            qint64 dirSize = 0;
            calculateDirectorySize(fileInfo.absoluteFilePath(), dirSize);
            folderSizes[fileName] = dirSize;
            totalSize += dirSize;
        }
    }

    // Добавление размера текущей директории
    qint64 currentDirectorySize = 0;
    // Суммирование размеров всех файлов верхнего уровня, которые не являются папками
    foreach (const QFileInfo &fileInfo, fileList) {
        currentDirectorySize += fileInfo.size();
    }

    // Добавление не папковых файлов в результат
    folderSizes["CurrentDirectory"] = currentDirectorySize;
    totalSize += currentDirectorySize;

    return folderSizes;
}


