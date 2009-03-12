#ifndef GFX_H
#define GFX_H

#include "misc.h"

class Gfx {
  SDL_Surface *screen;
  SDL_mutex *screen_mutex;
  Uint32 video_flags, screen_w, screen_h, screen_w_max, screen_h_max;
public:
  int screen_w_default, screen_h_default;

  Gfx();
  bool init();
  void update();

  void screen_locked(bool);
  void toggle_fullscreen();
  void toggle_highres();
  void draw_pixel(int,int,Uint32);
  void draw_rect(int,int,int,int,Uint32);
  int get_screen_w();
  int get_screen_h();
};
inline void Gfx::draw_pixel(int x, int y, Uint32 color) {
  //SDL_LockMutex(screen_mutex);
  *((Uint32*)screen->pixels + y * screen->pitch / 4 + x) = color;
  //SDL_UnlockMutex(screen_mutex);
}
inline void Gfx::draw_rect(int x, int y, int w, int h, Uint32 color) {
  //printf("x: %d, y: %d, w: %d, h: %d, color: %u\n",
  //       x, y, w, h, color);
  if (w * h == 1) {
    draw_pixel(x, y, color);
    return;
  }
  SDL_Rect rec; rec.x = x; rec.y = y; rec.w = w; rec.h = h;
  //SDL_LockMutex(screen_mutex);
  SDL_FillRect(screen, &rec, color);
  //SDL_UnlockMutex(screen_mutex);
}
inline int Gfx::get_screen_w() {
  return screen_w;
}
inline int Gfx::get_screen_h() {
  return screen_h;
}

#endif
