#pragma once

#include <cstdint>

struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct RGBA {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct HSL {
  float h;
  float s;
  float l;
};

struct HSLA {
  float h;
  float s;
  float l;
  float a;
};

HSL RGBtoHSL(const RGB &in);
HSLA RGBAtoHSLA(const RGBA &in);

RGB HSLtoRGB(const HSL &in);
RGBA HSLAtoRGBA(const HSLA &in);
