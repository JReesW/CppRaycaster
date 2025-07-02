#ifndef VINDICTA_LINES_H
#define VINDICTA_LINES_H

#include <optional>
#include <string>

#include <types.h>
#include <maths.h>

float distance(const point& a, const point& b);
float dot(const point& a, const point& b);

std::optional<point> intersection(const line& a, const line& b);

bool line_circle_intersect(const line& ln, const point& center, const float& radius);

float get_relative_angle(point& from, point& dir, point& to);

#endif
