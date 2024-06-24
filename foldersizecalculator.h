#ifndef FOLDERSIZECALCULATOR_H
#define FOLDERSIZECALCULATOR_H

#include "ISizeCalculator.h"
#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QDebug>

// Стратегия для вычисления размера файлов, сгруппированных по папкам
class FolderSizeCalculator : public ISizeCalculator {
public:
    QMap<QString, qint64> calculateSize(const QString &path, qint64 &totalSize) override;
};

#endif // FOLDERSIZECALCULATOR_H
