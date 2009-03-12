#ifndef PALETTE_H
#define PALETTE_H

#include "gfx.h"
#include "misc.h"

class Palette {
  private:
    int palette_size;
    Uint32 *palette;
    Uint32 bg_color;
  public:
    Palette();
    //Uint32 blend(Uint32, Uint32, double);
    Uint32 get_color(double iter);
};

inline Uint32 Palette::get_color(double iter) {
  if (iter < 0) return bg_color;
  float v = sqrt(iter)/24 * palette_size;
  union {
    Uint32 i;
    Uint8 b[4];
  } u1, u2;
  u1.i = palette[int(v) % palette_size];
  u2.i = palette[int(v+1) % palette_size];
  float a = cos((v - int(v)) * M_PI)/-2+.5;
  for (int i = 0; i < 3; i++) {
    u1.b[i] = (Uint8(u1.b[i] * (1-a) + u2.b[i] * a));
  }
  //if (pix->dist < -12) u1.b[i] += (255-u1.b[i])/2;

  return u1.i;
}

#endif
