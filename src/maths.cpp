#include <maths.h>

float maths::deg2rad(float deg) {
    return deg * (PI / 180.0);
}

float maths::deg2rad(int deg) {
    return (float)deg * (PI / 180.0);
}

float maths::clamp(const float& in, const float& min, const float& max) {
    return std::min(max, std::max(min, in));
}

int maths::clamp(const int& in, const int& min, const int& max) {
    return std::min(max, std::max(min, in));
}

float maths::lerp(const float& a, const float& b, const float& t) {
    return (1 - t) * a + t * b;
}

float maths::invlerp(const float& a, const float& b, const float& t) {
    if (a == b) return 0.0f;
    return (t - a) / (b - a);
}
