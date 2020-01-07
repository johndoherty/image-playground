#include "image_playground/color.h"
#include "image_playground/math.h"

#include <cmath>
#include <stdlib.h>

HSL RGBtoHSL(const RGB &in) {
  const float kEpsilon = 0.00001f;

  const float r = static_cast<float>(in.r) / 255.f;
  const float g = static_cast<float>(in.g) / 255.f;
  const float b = static_cast<float>(in.b) / 255.f;

  float min = r < g ? r : g;
  min = min < b ? min : b;

  float max = r > g ? r : g;
  max = max > b ? max : b;

  const float delta = max - min;
  const float l = (max + min) / 2.f;

  // Hue
  float h = 0.f;
  if (delta < kEpsilon) {
    h = 0;
  } else if (r >= max) {
    h = (g - b) / delta;
  } else if (g >= max) {
    h = 2.0 + ((b - r) / delta);
  } else {
    h = 4.0 + ((r - g) / delta);
  }

  h *= 60.0;

  if (h < 0.0) {
    h += 360.0;
  }

  // Saturation
  float s = 0.f;
  if (max < kEpsilon) {
    s = 0.f;
  } else if (min > (1.f - kEpsilon)) {
    s = 0.f;
  } else {
    const float inv_l = 1.f - l;
    const float d = l < inv_l ? l : inv_l;
    s = (max - l) / d;
  }

  return HSL{h, s, l};
}

HSLA RGBAtoHSLA(const RGBA &in) {
  RGB rgb{in.r, in.g, in.b};
  HSL hsl = RGBtoHSL(rgb);
  return HSLA{hsl.h, hsl.s, hsl.l, static_cast<float>(in.a) / 255.f};
}

RGB HSLtoRGB(const HSL &in) {
  const float chroma = (1.f - Abs((2.f * in.l) - 1.f)) * in.s;

  float hh = in.h >= 360.f ? 0.f : in.h;
  hh /= 60.0;

  const int hh_int = static_cast<int>(hh);
  const float hh_decimal = hh - static_cast<float>(hh_int);
  const float hh_mod_2 = static_cast<float>(hh_int % 2) + hh_decimal;

  const float x = chroma * (1.f - Abs(hh_mod_2 - 1.f));

  float r = 0.f;
  float g = 0.f;
  float b = 0.f;

  if (hh < 0) {
    // Nothing
  } else if (hh <= 1.f) {
    r = chroma;
    g = x;
  } else if (hh <= 2.f) {
    r = x;
    g = chroma;
  } else if (hh <= 3.f) {
    g = chroma;
    b = x;
  } else if (hh <= 4.f) {
    g = x;
    b = chroma;
  } else if (hh <= 5.f) {
    r = x;
    b = chroma;
  } else if (hh <= 6.f) {
    r = chroma;
    b = x;
  }

  const float offset = in.l - (chroma / 2.f);
  return RGB{static_cast<uint8_t>((r + offset) * 255.f),
             static_cast<uint8_t>((g + offset) * 255.f),
             static_cast<uint8_t>((b + offset) * 255.f)};
}

RGBA HSLAtoRGBA(const HSLA &in) {
  const HSL hsl{in.h, in.s, in.l};
  const RGB rgb = HSLtoRGB(hsl);
  return RGBA{rgb.r, rgb.g, rgb.b, static_cast<uint8_t>(in.a * 255.f)};
}
