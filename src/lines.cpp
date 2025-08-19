#include <lines.h>

///////////////////
// POINT METHODS //
///////////////////

Point Point::rotate(float angle) const {
    // directional vector rotation
    auto [px, py] = *this;

    float rad = maths::deg2rad(angle);
    float cs = cos(rad);
    float sn = sin(rad);

    float nx = px * cs - py * sn;
    float ny = px * sn + py * cs;

    return {nx, ny};
}

Point Point::operator+(const Point& a) const {
    return {a.x + x, a.y + y};
}

Point Point::operator-(const Point& a) const {
    return {x - a.x, y - a.y};
}

Point Point::operator*(const float& a) const {
    return {x * a, y * a};
}

float distance(const Point& a, const Point& b) {
    return sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

float dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y + b.y;
}

//////////////////
// LINE METHODS //
//////////////////

Line Line::rotate(float angle) const {
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

std::optional<Point> intersection(const Line& a, const Line& b) {
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
    Point p = {x, y};
    
    return p;
}

bool line_circle_intersect(const Line& ln, const Point& center, const float& radius) {  // Checks whether a given line and circle intersect
    // Only checks whether they intersect, not where
    auto [a, b] = ln;
    const float l2 = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));

    // if a == b, then the line segment is just a point, return distance from point to circle center
    if(l2 == 0.0) return distance(a, center);

    const float t = std::max(0.0f, std::min(1.0f, dot(center - a, b - a) / l2));
    const Point projection = a + ((b - a) * t);
    return distance(center, projection) <= radius;
}

float get_relative_angle(Point& from, Point& dir, Point& to) {
    float vx = to.x - from.x;
    float vy = to.y - from.y;

    float len = sqrt(vx*vx + vy*vy);
    vx /= len;
    vy /= len;

    float dot = dir.x * vx + dir.y * vy;
    if (dot < -1.0f) dot = -1.0f;
    if (dot > 1.0f) dot = 1.0f;
    float angle = acos(dot);

    float cross = dir.x * vy - dir.y * vx;
    if (cross < 0)
        angle = -angle;

    return angle; // in radians
}
