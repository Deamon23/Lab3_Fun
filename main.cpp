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

// Функция для вычисления процентов от общего размера
QMap<QString, double> calculatePercentages(QMap<QString, qint64> &sizes, qint64 totalSize, double minPer) {
    QMap<QString, double> percentages;
    double otherPerc = 0;
    double otherSize = 0;
    for (auto it = sizes.begin(); it != sizes.end(); ++it) {
        double percentage = (static_cast<double>(it.value()) / totalSize) * 100;
        if (percentage < minPer){
            otherPerc += percentage;
            otherSize += it.value();
        }
        percentages[it.key()] = percentage;
    }
    if (otherSize>0)
    {
        percentages["other"] = otherPerc;
        sizes["other"] = otherSize;
    }
    return percentages;
}

void printSizesAndPercentages(const QMap<QString, qint64> &sizes, const QMap<QString, double> &percentages, double minPer) {
    for (auto it = sizes.begin(); it != sizes.end(); ++it) {
        if ((percentages[it.key()] > minPer || it.key() == "other") && percentages[it.key()]!=0) {
            qDebug() << it.key() << ":" << it.value() << "байт" << "(" << formatPercentage(percentages[it.key()]) << ")";
        }
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString path = "C:/gits/Lab3_Fun/Testing1";
    qint64 totalSize = 0;
    double minPer = 0.01;
    // Создание контекста
    Context context;

    // Установка стратегии для группировки по папкам
    context.setStrategy(std::make_unique<FolderSizeCalculator>());
    QMap<QString, qint64> folderSizes = context.calculateSize(path, totalSize);
    QMap<QString, double> folderPercentages = calculatePercentages(folderSizes, totalSize, minPer);
    qDebug() << "Размеры файлов, сгруппированные по папкам:";
    printSizesAndPercentages(folderSizes, folderPercentages, minPer);

    // Сброс общего размера для следующего расчета
    totalSize = 0;

    // Установка стратегии для группировки по типам
    context.setStrategy(std::make_unique<TypeSizeCalculator>());
    QMap<QString, qint64> typeSizes = context.calculateSize(path, totalSize);
    QMap<QString, double> typePercentages = calculatePercentages(typeSizes, totalSize, minPer);
    qDebug() << "Размеры файлов, сгруппированные по типам:";
    printSizesAndPercentages(typeSizes, typePercentages, minPer);

    return 0;
}
// C:/gits/Lab3_Fun/Testing1
