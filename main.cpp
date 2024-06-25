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

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString path = "C:/gits/Lab3_Fun/Testing1";
    qint64 totalSize = 0;
    double minPer = 2;
    // Создание контекста
    Context context;

    // Установка стратегии для группировки по папкам
    context.setStrategy(std::make_unique<FolderSizeCalculator>());
    QMap<QString, qint64> folderSizes = context.calculateSize(path, totalSize);
    QMap<QString, double> folderPercentages = calculatePercentages(folderSizes, totalSize, minPer);
    qDebug() << "Размеры файлов, сгруппированные по папкам:";
    QMap<QString, qint64>::const_iterator it1 = folderSizes.constBegin();
    QMap<QString, double>::const_iterator it2 = folderPercentages.constBegin();

    while (it1 != folderSizes.constEnd() && it2 != folderPercentages.constEnd()) {
        if(it2.value() > minPer or (it1.key() == "other"))
        {
            qDebug() << it1.key() << ":" << it1.value() << "байт" << "(" << formatPercentage(it2.value()) << ")";
        }
        ++it1;
        ++it2;
    }
    // Сброс общего размера для следующего расчета
    totalSize = 0;

    // Установка стратегии для группировки по типам
    context.setStrategy(std::make_unique<TypeSizeCalculator>());
    QMap<QString, qint64> typeSizes = context.calculateSize(path, totalSize);
    QMap<QString, double> TypePercentages = calculatePercentages(typeSizes, totalSize, minPer);
    qDebug() << "Размеры файлов, сгруппированные по типам:";

    QMap<QString, qint64>::const_iterator it3 = typeSizes.constBegin();
    QMap<QString, double>::const_iterator it4 = TypePercentages.constBegin();

    while (it3 != typeSizes.constEnd() && it4 != TypePercentages.constEnd()) {
        if(it4.value() > minPer or (it3.key() == "other"))
        {
            qDebug() << it3.key() << ":" << it3.value() << "байт" << "(" << formatPercentage(it4.value()) << ")";
        }
        ++it3;
        ++it4;
    }

    return a.exec();
}
// C:/gits/Lab3_Fun/Testing1
