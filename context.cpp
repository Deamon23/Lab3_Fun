#include "Context.h"

void Context::setStrategy(std::unique_ptr<ISizeCalculator> strategy) {
    this->strategy = std::move(strategy);
}

QMap<QString, qint64> Context::calculateSize(const QString &path, qint64 &totalSize) {
    if (strategy) {
        return strategy->calculateSize(path, totalSize);
    }
    return {};
}
