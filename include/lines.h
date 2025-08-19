#ifndef VINDICTA_LINES_H
#define VINDICTA_LINES_H

#include <optional>
#include <string>

#include <types.h>
#include <maths.h>

float distance(const Point& a, const Point& b);
float dot(const Point& a, const Point& b);

std::optional<Point> intersection(const Line& a, const Line& b);

bool line_circle_intersect(const Line& ln, const Point& center, const float& radius);

float get_relative_angle(Point& from, Point& dir, Point& to);

#endif
