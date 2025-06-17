#ifndef VINDICTA_MATHS_H
#define VINDICTA_MATHS_H

#include <cmath>

const float PI = 3.141592653;
struct point;

namespace maths {
    float deg2rad(float deg);
    float deg2rad(int deg);
    float clamp(const float& in, const float& min, const float& max);
    int clamp(const int& in, const int& min, const int& max);
    float lerp(const float& a, const float& b, const float& t);
    float invlerp(const float& a, const float& b, const float& t);
}

#endif
