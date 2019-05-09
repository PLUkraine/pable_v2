#pragma once
#include <stdlib.h>
#include <vector>

#define UNUSED_PARAM(x) (void)x

using Number = int;
using NodeRef = int;
using Operator = char;


inline void clearVector(std::vector<int>& v, int value = 0x0)
{
    memset(&v[0], value, v.size() * sizeof v[0]);
}
