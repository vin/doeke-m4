#include "gfx.h"

Gfx::Gfx() {
  screen_mutex = SDL_CreateMutex();
  screen_w_default = 800;
  screen_h_default = 600;
  video_flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
}
bool Gfx::init() {
  screen_w = screen_w_default;
  screen_h = screen_h_default;
  const SDL_VideoInfo* video_info = SDL_GetVideoInfo();
  screen_w_max = video_info->current_w;
  screen_h_max = video_info->current_h;
  screen = SDL_SetVideoMode(screen_w, screen_h, 32, video_flags);
  if (screen == NULL) return false;
  SDL_WM_SetCaption("Near Infinite Zoom", NULL);
  //font = TTF_OpenFont("Arial.ttf", 14);
  //char text[] = "Initializing...";
  //Set_text(text);
  return true;
}
void Gfx::update() {
  //SDL_Rect txt_rect = { 0, scr_h-surf_txt->h, 0, 0 };
  //Draw_rect(0, scr_h-surf_txt->h, scr_w, surf_txt->h, 0xffffff);
  //SDL_BlitSurface(surf_txt, NULL, screen, &txt_rect);

  //SDL_LockMutex(screen_mutex);
  SDL_Flip(screen);
  //SDL_UnlockMutex(screen_mutex);
}

void Gfx::screen_locked(bool lock) {
  if (!SDL_MUSTLOCK(screen)) return;
  if (lock) SDL_LockSurface(screen);
  else SDL_UnlockSurface(screen);
}
void Gfx::toggle_fullscreen() {
  video_flags ^= SDL_FULLSCREEN;
  if (screen_w == screen_w_max) {
    screen_w = screen_w_default;
    screen_h = screen_h_default;
  } else {
    screen_w = screen_w_max;
    screen_h = screen_h_max;
  }
  SDL_LockMutex(screen_mutex);
  screen = SDL_SetVideoMode(screen_w, screen_h, 32, video_flags);
  SDL_UnlockMutex(screen_mutex);
}
void Gfx::toggle_highres() {
  video_flags |= SDL_FULLSCREEN;
  if (screen_w == screen_w_max) {
    screen_w = screen_w_default;
    screen_h = screen_h_default;
  } else {
    screen_w = screen_w_max;
    screen_h = screen_h_max;
  }
  SDL_LockMutex(screen_mutex);
  screen = SDL_SetVideoMode(screen_w, screen_h, 32, video_flags);
  SDL_UnlockMutex(screen_mutex);
}
/*void Set_text(char str[]) {
  SDL_Color txtcolor = {0,0,0,0};
  SDL_Color bgcolor = {0xff, 0xff, 0xff, 0xff};
  surf_txt = TTF_RenderText_Shaded(font, str, txtcolor, bgcolor);
}*/

