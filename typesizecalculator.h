#ifndef TYPESIZECALCULATOR_H
#define TYPESIZECALCULATOR_H

#include "ISizeCalculator.h"
#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QDebug>

// Стратегия для вычисления размера файлов, сгруппированных по типам
class TypeSizeCalculator : public ISizeCalculator {
public:
    QMap<QString, qint64> calculateSize(const QString &path, qint64 &totalSize) override;
};

#endif // TYPESIZECALCULATOR_H
