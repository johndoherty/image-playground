#pragma once

template <typename T> T Max(const T &a, const T &b);
template <typename T> T Min(const T &a, const T &b);

template <typename T> T Abs(const T &a);
template <typename T> T AbsDiff(const T &a, const T &b);

float Random(float min, float max);
int Random(int min, int max);

#include "image_playground/templates/math.tpp"