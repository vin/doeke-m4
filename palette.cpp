#include "palette.h"

Palette::Palette() {
  bg_color = 0x0;
  palette_size = 6;
  palette = new Uint32[palette_size];
  palette[0] = 0x00065c;
  palette[1] = 0x0b38ab;
  palette[2] = 0xffffff;
  palette[3] = 0xff6600;
  palette[4] = 0xa53c0f;
  palette[5] = 0x090349;
  /*
  / *
  palette_size = 6;
  palette = new Uint32[palette_size];
  palette[0] = 0xff0000;
  palette[1] = 0xffff00;
  palette[2] = 0x00ff00;
  palette[3] = 0x00ffff;
  palette[4] = 0x0000ff;
  palette[5] = 0xff00ff;
  / *
  palette_size = 2;
  palette = new Uint32[palette_size];
  palette[0] = 0xffffff;
  palette[1] = 0x000000;
  */
}
