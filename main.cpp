#include "misc.h"
#include "gfx.h"
#include "palette.h"
#include "input.h"
#include "plotter.h"

Gfx gfx;
Input input;
Plotter plotter;
Palette palette;
const unsigned FPS = 20;

int frame_time;

double zoom_factor = 2;

int main(int argc, char *argv[]) {
  FILE* fp = fopen("config.txt", "r");
  if (fp) {
    int a, b;
    if (fscanf(fp," cores %d", &a) == 1) plotter.n_threads = a;
    if (fscanf(fp," def_res %d %d", &a, &b) == 2) {
      gfx.screen_w_default = a;
      gfx.screen_h_default = b;
    }
    if (fscanf(fp," max_iter %d", &a) == 1) plotter.max_iter = a;
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1 || !gfx.init()) return 1;

  plotter.init();
  plotter.resize(gfx.get_screen_w(), gfx.get_screen_h());
  plotter.plot();

  while (true) {
    frame_time = SDL_GetTicks();
    input.update();

    if (input.key_pressed(SDLK_ESCAPE) || input.is_quitting())
      break;

    if (input.key_down( SDLK_LCTRL ) &&
               input.mouse_pressed(SDL_BUTTON_LEFT)) {
      plotter.center();
    } else if (input.mouse_pressed(SDL_BUTTON_LEFT)) {
      if (input.key_down(SDLK_LSHIFT)) {
        plotter.zoom(zoom_factor * 5);
      } else {
        plotter.zoom(zoom_factor);
      }
    } else if (input.mouse_pressed(SDL_BUTTON_RIGHT)) {
      if (input.key_down(SDLK_LSHIFT)) {
        plotter.zoom(1/(zoom_factor * 5));
      } else {
        plotter.zoom(1/zoom_factor);
      }
    }

    if (input.key_pressed(SDLK_f)) {
      plotter.end_plotting();
      gfx.toggle_fullscreen();
      plotter.resize(gfx.get_screen_w(), gfx.get_screen_h());
      plotter.plot();
      //zoom_on();
    }

    if (input.key_pressed(SDLK_p)) {
      plotter.print_pos();
    }

    gfx.update();
    int time_rem = 1000 / FPS - (SDL_GetTicks() - frame_time);
    if (time_rem >= 5)
      SDL_Delay(time_rem);
  }
  plotter.end_plotting();

  SDL_Quit();
  return 0;
}
