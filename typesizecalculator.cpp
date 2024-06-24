#include "TypeSizeCalculator.h"

QMap<QString, qint64> TypeSizeCalculator::calculateSize(const QString &path, qint64 &totalSize) {
    QMap<QString, qint64> typeSizes;
    QDir dir(path);

    // Получение списка файлов в текущей папке
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    // Обход каждого файла в папке
    foreach (const QFileInfo &fileInfo, fileList) {
        QString fileType = "*." + fileInfo.suffix();  // Определение типа файла по его расширению
        qint64 fileSize = fileInfo.size();

        // Суммирование размеров файлов одного типа
        if (typeSizes.contains(fileType)) {
            typeSizes[fileType] += fileSize;
        } else {
            typeSizes[fileType] = fileSize;
        }
        totalSize += fileSize;
    }

    // Получение списка подкаталогов
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Рекурсивный обход каждого подкаталога
    foreach (const QFileInfo &folderInfo, folderList) {
        QMap<QString, qint64> subFolderSizes = calculateSize(folderInfo.absoluteFilePath(), totalSize);
        for (auto it = subFolderSizes.begin(); it != subFolderSizes.end(); ++it) {
            if (typeSizes.contains(it.key())) {
                typeSizes[it.key()] += it.value();
            } else {
                typeSizes[it.key()] = it.value();
            }
        }
    }

    return typeSizes;
}
