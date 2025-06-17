#ifndef VINDICTA_LINES_H
#define VINDICTA_LINES_H

#include <optional>
#include <string>

#include <maths.h>

struct point {  // either a coordinate or a directional vector
    float x;
    float y;

    point rotate(float angle) const;
    point operator+(const point& a) const;
    point operator-(const point& a) const;
    point operator*(const float& a) const;
};

struct line {  // line segment with defined start and end points
    point start;
    point end;
    
    line rotate(float angle) const;
};

struct mapline {  // line segment with extra game information
    line geometry;
    std::string image;
};

float distance(const point& a, const point& b);
float dot(const point& a, const point& b);

std::optional<point> intersection(const line& a, const line& b);

bool line_circle_intersect(const line& ln, const point& center, const float& radius);

#endif
