#pragma once

#include <QList>
#include <memory>
#include <optional>
#include "cellindex.h"


int fuzzyCmp(float a, float b, const float EPS = 1e-6);
QDebug operator<<(QDebug d, const std::optional<int> &v);
QDebug operator<<(QDebug d, const CellIndex &v);

