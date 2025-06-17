#include <lines.h>

///////////////////
// POINT METHODS //
///////////////////

point point::rotate(float angle) const {
    // directional vector rotation
    auto [x, y] = *this;

    float rad = maths::deg2rad(angle);
    float cs = cos(rad);
    float sn = sin(rad);

    float nx = x * cs - y * sn;
    float ny = x * sn + y * cs;

    return {nx, ny};
}

point point::operator+(const point& a) const {
    return {a.x + x, a.y + y};
}

point point::operator-(const point& a) const {
    return {x - a.x, y - a.y};
}

point point::operator*(const float& a) const {
    return {x * a, y * a};
}

float distance(const point& a, const point& b) {
    return sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

float dot(const point& a, const point& b) {
    return a.x * b.x + a.y + b.y;
}

//////////////////
// LINE METHODS //
//////////////////

line line::rotate(float angle) const {
    auto [sx, sy] = this->start;
    auto [ex, ey] = this->end;
    float vx = ex - sx;
    float vy = ey - sy;

    float rad = maths::deg2rad(angle);
    float cs = cos(rad);
    float sn = sin(rad);

    float nx = vx * cs - vy * sn;
    float ny = vx * sn + vy * cs;

    return {{sx, sy}, {sx + nx, sy + ny}};
}

std::optional<point> intersection(const line& a, const line& b) {
    auto [x1, y1] = a.start;
    auto [x2, y2] = a.end;
    auto [x3, y3] = b.start;
    auto [x4, y4] = b.end;
    float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

    if (denom == 0.0) return std::nullopt;  // parallel

    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    
    if (ua < 0.0 || ua > 1) return std::nullopt;  // out of range
        
    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
    if (ub < 0 || ub > 1) return std::nullopt;  // out of range

    float x = x1 + ua * (x2 - x1);
    float y = y1 + ua * (y2 - y1);
    point p = {x, y};
    
    return p;
}

bool line_circle_intersect(const line& ln, const point& center, const float& radius) {  // Checks whether a given line and circle intersect
    // Only checks whether they intersect, not where
    auto [a, b] = ln;
    const float l2 = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));

    // if a == b, then the line segment is just a point, return distance from point to circle center
    if(l2 == 0.0) return distance(a, center);

    const float t = std::max(0.0f, std::min(1.0f, dot(center - a, b - a) / l2));
    const point projection = a + ((b - a) * t);
    return distance(center, projection) <= radius;
}
