#include <QCoreApplication>
#include <QDebug>
#include "Context.h"
#include "FolderSizeCalculator.h"
#include "TypeSizeCalculator.h"

// Функция для форматирования процента с точностью до двух знаков после запятой
QString formatPercentage(double percentage) {
    if (percentage > 0 && percentage < 0.01) {
        return "< 0.01%";
    }
    return QString::number(percentage, 'f', 2) + "%";
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString path = "C:/gits/Lab3_Fun/Testing1";
    qint64 totalSize = 0;

    // Создание контекста
    Context context;

    // Установка стратегии для группировки по папкам
    context.setStrategy(std::make_unique<FolderSizeCalculator>());
    QMap<QString, qint64> folderSizes = context.calculateSize(path, totalSize);
    qDebug() << "Размеры файлов, сгруппированные по папкам:";
    foreach (const QString &folderName, folderSizes.keys()) {
        double percentage = (static_cast<double>(folderSizes[folderName]) / totalSize) * 100;
        qDebug() << folderName << ":" << folderSizes[folderName] << "байт" << "(" << formatPercentage(percentage) << ")";
    }

    // Сброс общего размера для следующего расчета
    totalSize = 0;

    // Установка стратегии для группировки по типам
    context.setStrategy(std::make_unique<TypeSizeCalculator>());
    QMap<QString, qint64> typeSizes = context.calculateSize(path, totalSize);
    qDebug() << "Размеры файлов, сгруппированные по типам:";
    foreach (const QString &fileType, typeSizes.keys()) {
        double percentage = (static_cast<double>(typeSizes[fileType]) / totalSize) * 100;
        qDebug() << fileType << ":" << typeSizes[fileType] << "байт" << "(" << formatPercentage(percentage) << ")";
    }

    return a.exec();
}
// C:/gits/Lab3_Fun/Testing1
