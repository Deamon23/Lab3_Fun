#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QDebug>
#include <iomanip>
#include <iostream>

// Функция для получения размера файла в байтах
qint64 getFileSizeInBytes(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    return fileInfo.size();
}

// Функция для вычисления общего размера файлов в папке
QMap<QString, qint64> calculateSizeGroupedByFolders(const QString &path, qint64 &totalSize) {
    QMap<QString, qint64> folderSizes;
    qint64 CurrentDirectorySize = 0;
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
            folderTotalSize += getFileSizeInBytes(fileInfo.absoluteFilePath());
        }

        // Сохранение общего размера файлов в подкаталоге
        folderSizes[folderInfo.fileName()] = folderTotalSize;
        totalSize += folderTotalSize;
    }

    // Суммирование размеров всех файлов верхнего уровня, которые не являются папками
    foreach (const QFileInfo &fileInfo, fileList) {
        CurrentDirectorySize += getFileSizeInBytes(fileInfo.absoluteFilePath());
    }

    // Добавление не папковых файлов в результат
    folderSizes["CurrentDirectory"] = CurrentDirectorySize;
    totalSize += CurrentDirectorySize;

    return folderSizes;
}

// Рекурсивная функция для вычисления размера файлов в папке и подкаталогах, сгруппированных по типам
void calculateSizeByTypesInFolder(const QString &path, QMap<QString, qint64> &typeSizes) {
    QDir dir(path);

    // Получение списка файлов в текущей папке
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    // Обход каждого файла в папке
    foreach (const QFileInfo &fileInfo, fileList) {
        QString fileType = "*." + fileInfo.suffix();  // Определение типа файла по его расширению
        qint64 fileSize = getFileSizeInBytes(fileInfo.absoluteFilePath());

        // Суммирование размеров файлов одного типа
        if (typeSizes.contains(fileType)) {
            typeSizes[fileType] += fileSize;
        } else {
            typeSizes[fileType] = fileSize;
        }
    }

    // Получение списка подкаталогов
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Рекурсивный обход каждого подкаталога
    foreach (const QFileInfo &folderInfo, folderList) {
        calculateSizeByTypesInFolder(folderInfo.absoluteFilePath(), typeSizes);
    }
}

// Функция для вычисления общего размера файлов, сгруппированных по типам
QMap<QString, qint64> calculateSizeGroupedByTypes(const QString &path) {
    QMap<QString, qint64> typeSizes;
    QDir dir(path);

    // Проверка существования директории
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << path;
        return typeSizes;
    }

    // Запуск рекурсивного вычисления размеров файлов по типам
    calculateSizeByTypesInFolder(path, typeSizes);
    return typeSizes;
}

// Функция для форматирования процента с точностью до двух знаков после запятой
QString formatPercentage(double percentage) {
    if (percentage > 0 && percentage < 0.01) {
        return "< 0.01%";
    }
    return QString::number(percentage, 'f', 2) + "%";
}

int main() {
    QString path = "C:/gits/Lab3_Fun/Testing1";
    qint64 totalSize = 0;

    // Группировка по папкам
    QMap<QString, qint64> folderSizes = calculateSizeGroupedByFolders(path, totalSize);
    qDebug() << "Размеры файлов, сгруппированные по папкам:";
    foreach (const QString &folderName, folderSizes.keys()) {
        double percentage = (static_cast<double>(folderSizes[folderName]) / totalSize) * 100;
        qDebug() << folderName << ":" << folderSizes[folderName] << "байт" << "(" << formatPercentage(percentage) << ")";
    }

    // Группировка по типам
    QMap<QString, qint64> typeSizes = calculateSizeGroupedByTypes(path);
    qDebug() << "Размеры файлов, сгруппированные по типам:";
    foreach (const QString &fileType, typeSizes.keys()) {
        double percentage = (static_cast<double>(typeSizes[fileType]) / totalSize) * 100;
        qDebug() << fileType << ":" << typeSizes[fileType] << "байт" << "(" << formatPercentage(percentage) << ")";
    }

    return 0;
}
// C:/gits/Lab3_Fun/Testing1
