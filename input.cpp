#include <iostream>

#include "input.h"

Input::Input() {
  SDL_EnableKeyRepeat(0, 0);
  memset(keys_down, 0, sizeof(keys_down));
  memset(keys_pressed, 0, sizeof(keys_pressed));
  memset(keys_released, 0, sizeof(keys_released));
  mouse_down_ = mouse_pressed_ = mouse_released_ = 0;
  quitting = false;
}
void Input::update() {
  memset(keys_pressed, 0, sizeof(keys_pressed));
  memset(keys_released, 0, sizeof(keys_released));
  mouse_pressed_ = mouse_released_ = 0;
  SDL_GetMouseState(&mouse_x_, &mouse_y_);

  quitting = false;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_MOUSEBUTTONDOWN:
        mouse_down_ |= SDL_BUTTON(event.button.button);
        mouse_pressed_ |= SDL_BUTTON(event.button.button);
        break;
      case SDL_MOUSEBUTTONUP:
        mouse_down_ &= ~SDL_BUTTON(event.button.button);
        mouse_released_ |= SDL_BUTTON(event.button.button);
        break;
      case SDL_KEYDOWN:
        keys_down[event.key.keysym.sym] = true;
        keys_pressed[event.key.keysym.sym] = true;
        break;
      case SDL_KEYUP:
        keys_down[event.key.keysym.sym] = false;
        keys_released[event.key.keysym.sym] = true;
        break;
      case SDL_QUIT:
        quitting = true;
    }
  }
  //return (!quitting && !key_pressed(SDLK_ESCAPE));
}
