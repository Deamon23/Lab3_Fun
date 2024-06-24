#ifndef ISIZECALCULATOR_H
#define ISIZECALCULATOR_H

#include <QString>
#include <QMap>

// Интерфейс для стратегий вычисления размера
class ISizeCalculator {
public:
    virtual ~ISizeCalculator() = default;
    virtual QMap<QString, qint64> calculateSize(const QString &path, qint64 &totalSize) = 0;
};

#endif // ISIZECALCULATOR_H
