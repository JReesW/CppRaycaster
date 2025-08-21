#include <lines.hpp>

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
    return a.x * b.x + a.y * b.y;
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
    Point ab = ln.end - ln.start;
    Point ac = center - ln.start;
    Point ad = ab * (dot(ab, ac) / dot(ab, ab));
    Point d = ln.start + ad;
    if (distance(center, d) <= radius) {
        float len = distance(ln.start, ln.end);
        if (distance(ln.start, d) <= len && distance(ln.end, d) <= len) {
            return true;
        }
    }
    return false;
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

Point normal(const Line& ln) {
    // right-handed normal
    float dist = distance(ln.start, ln.end);
    float dx = (ln.end.x - ln.start.x) / dist;
    float dy = (ln.end.y - ln.start.y) / dist;

    return Point{-dy, dx};
}

Point midpoint(const Line& ln) {
    float mx = (ln.start.x + ln.end.x) / 2;
    float my = (ln.start.y + ln.end.y) / 2;

    return Point{mx, my};
}
