#ifndef CONTEXT_H
#define CONTEXT_H

#include "ISizeCalculator.h"
#include <memory>

// Контекст, использующий стратегии
class Context {
public:
    void setStrategy(std::unique_ptr<ISizeCalculator> strategy);
    QMap<QString, qint64> calculateSize(const QString &path, qint64 &totalSize);

private:
    std::unique_ptr<ISizeCalculator> strategy;
};

#endif // CONTEXT_H
