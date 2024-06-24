#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QDebug>

// Функция для получения размера файла в байтах
qint64 getFileSizeInBytes(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    return fileInfo.size();
}

// Функция для вычисления общего размера файлов в папке без углубления в подкаталоги
QMap<QString, qint64> calculateSizeGroupedByFolders(const QString &path) {
    QMap<QString, qint64> folderSizes;
    qint64 unknownSize = 0;
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
        qint64 totalSize = 0;
        QDir folderDir(folderInfo.absoluteFilePath());

        // Получение списка файлов в текущем подкаталоге
        QFileInfoList fileList = folderDir.entryInfoList(QDir::Files | QDir::NoSymLinks);

        // Суммирование размеров всех файлов в текущем подкаталоге
        foreach (const QFileInfo &fileInfo, fileList) {
            totalSize += getFileSizeInBytes(fileInfo.absoluteFilePath());
        }

        // Сохранение общего размера файлов в подкаталоге
        folderSizes[folderInfo.fileName()] = totalSize;
    }

    // Суммирование размеров всех файлов верхнего уровня, которые не являются папками
    foreach (const QFileInfo &fileInfo, fileList) {
        unknownSize += getFileSizeInBytes(fileInfo.absoluteFilePath());
    }

    // Добавление неизвестных файлов в результат
    folderSizes["CurrentDirectory"] = unknownSize;

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

int main() {
    QString path = "C:/gits/Lab3_Fun/Testing1";

    // Пример использования функции для группировки по папкам
    QMap<QString, qint64> folderSizes = calculateSizeGroupedByFolders(path);
    qDebug() << "Размеры файлов, сгруппированные по папкам:";
    foreach (const QString &folderName, folderSizes.keys()) {
        qDebug() << folderName << ":" << folderSizes[folderName] << "байт";
    }

    // Пример использования функции для группировки по типам
    QMap<QString, qint64> typeSizes = calculateSizeGroupedByTypes(path);
    qDebug() << "Размеры файлов, сгруппированные по типам:";
    foreach (const QString &fileType, typeSizes.keys()) {
        qDebug() << fileType << ":" << typeSizes[fileType] << "байт";
    }

    return 0;
}
