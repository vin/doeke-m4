#ifndef PLOTTER_H
#define PLOTTER_H

#include "misc.h"
#include "gfx.h"
#include "palette.h"
#include "input.h"

const int square_size = 128;

struct Square {
  float pix[square_size][square_size];
  int step, fase, x_offset,
  y_offset, x_max, y_max;

  Square() {}
  Square(int a, int b, int c, int d):
         x_offset(a), y_offset(b),
         x_max(c), y_max(d) {
    reset();
  }
  bool advance() {
    if (--fase < 1) {
      fase = 3;
      step >>= 1;
    }
    return (step > 0);
  }
  void reset() {
    step = 8;
    fase = 4;
  }
};

class Thread {
  private:
    bool running;

  public:
    Thread();
    int run();
    void stop();
};

class Plotter {
  private:
    Square *squares;
    SDL_mutex *queue_mutex;
    std::deque<Square*> queue;
    Thread *threads;
    SDL_Thread **sdl_threads;
    
    double magni, r_step, i_step,
           min_r, min_i, max_r, max_i;

  public:
    int max_iter, n_squares, n_threads;

    Plotter();
    void init();
    double iterate(double, double);
    void resize(int, int);
    void plot();
    void end_plotting();
    void print_pos();
    void zoom(double);
    void center();
    Square *get_square(Square*);
    double x_to_r(int);
    double y_to_i(int);
};

inline double Plotter::x_to_r(int x) {
  return min_r + x * r_step;
}
inline double Plotter::y_to_i(int y) {
  return min_i + y * i_step;
}

int thread_launcher(void*);

inline double Plotter::iterate(double cr, double ci) {
  double zr = cr, zi = ci, zr2 = zr*zr, zi2 = zi*zi;
  int iter;
  for (iter = 0; iter < max_iter && zr2 + zi2 < 40.; iter++) {
    zi = (zr + zr) * zi + ci;
    zr = zr2 - zi2 + cr;
    zr2 = zr * zr;
    zi2 = zi * zi;
  }
  if (iter >= max_iter) return -1;
  double smooth_iter = iter - log(log(sqrt(zr2+zi2))) /  0.6931;
  return std::max(0., smooth_iter);
}

#endif
