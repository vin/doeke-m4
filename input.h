#ifndef INPUT_H
#define INPUT_H

#include <iostream>

#include "misc.h"

class Input {
  Uint8 mouse_down_, mouse_pressed_, mouse_released_;
  bool keys_down[500], keys_pressed[500], keys_released[500];
  int mouse_x_, mouse_y_;
  bool quitting;
public:
  Input();
  void update();
  bool key_down(int);
  bool key_pressed(int);
  bool key_released(int);
  bool mouse_down(int);
  bool mouse_pressed(int);
  bool mouse_released(int);
  int mouse_x();
  int mouse_y();
  bool is_quitting();
};
inline bool Input::key_down(int key) {
  return keys_down[key];
}
inline bool Input::key_pressed(int key) {
  return keys_pressed[key];
}
inline bool Input::key_released(int key) {
  return keys_released[key];
}
inline bool Input::mouse_down(int button) {
  return (mouse_down_ & SDL_BUTTON(button));
}
inline bool Input::mouse_pressed(int button) {
  return (mouse_pressed_ & SDL_BUTTON(button));
}
inline bool Input::mouse_released(int button) {
  return (mouse_released_ & SDL_BUTTON(button));
}
inline int Input::mouse_x() {
  return mouse_x_;
}
inline int Input::mouse_y() {
  return mouse_y_;
}
inline bool Input::is_quitting() {
  return quitting;
}


#endif
