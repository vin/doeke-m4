#include "plotter.h"

extern Gfx gfx;
extern Plotter plotter;
extern Palette palette;
extern Input input;

Plotter::Plotter() {
  max_iter = 10;
  magni = 3;
  min_r = -2.5;
  min_i = -1.5;
  n_threads = 1;
}

void Plotter::init() {
  squares = NULL;
  queue_mutex = SDL_CreateMutex();
  threads = new Thread[n_threads];
  sdl_threads = new SDL_Thread*[n_threads];
  for (int i = 0; i < n_threads; i++) {
    threads[i] = Thread();
    sdl_threads[i] = NULL;
  }
}

void Plotter::resize(int width, int height) {
  delete squares;
  int h_squares = int(ceil(double(width) / square_size));
  int v_squares = int(ceil(double(height) / square_size));
  n_squares = h_squares * v_squares;
  squares = new Square[n_squares];
  for (int x = 0; x < h_squares; x ++)
  for (int y = 0; y < v_squares; y ++) {
    squares[y*h_squares+x] = Square(x*square_size, y*square_size,
                                    std::min(width-x*square_size, square_size),
                                    std::min(height-y*square_size, square_size));
  }
}

void Plotter::end_plotting() {
  for (int i = 0; i < n_threads; i++) {
    if (sdl_threads[i] == NULL) continue;
    threads[i].stop();
    SDL_WaitThread(sdl_threads[i], NULL);
    sdl_threads[i] = NULL;
  }
}

void Plotter::plot() {
  max_i = min_i + magni;
  max_r = min_r + magni * gfx.get_screen_w() / gfx.get_screen_h();
  r_step = (max_r - min_r) / (gfx.get_screen_w() - 1);
  i_step = (max_i - min_i) / (gfx.get_screen_h() - 1);

  end_plotting();
  queue.clear();
  for (int i = 0; i < n_squares; i++) {
    squares[i].reset();
    queue.push_back(squares+i);
  }
  for (int i = 0; i < n_threads; i++) {
    sdl_threads[i] = SDL_CreateThread(thread_launcher,
                                      (void*)(threads+i));
  }
}

void Plotter::print_pos() {
  printf("magni: %lf\nmin_r: %lf\nmin_i: %lf\n", magni, min_r, min_i);
}

void Plotter::center() {
  end_plotting();
  min_r = x_to_r(input.mouse_x()) - (max_r - min_r) / 2;
  min_i = y_to_i(input.mouse_y()) - (max_i - min_i) / 2;
  plot();
}

void Plotter::zoom(double factor) {
  end_plotting();
  min_r = x_to_r(input.mouse_x()) - (max_r - min_r) / factor / 2;
  min_i = y_to_i(input.mouse_y()) - (max_i - min_i) / factor / 2;
  magni /= factor;
  plot();
}

Square* Plotter::get_square(Square *old_square) {
  SDL_LockMutex(queue_mutex);
  Square *square;
  if (old_square && old_square->advance()) {
    queue.push_back(old_square);
  }
  //printf("queue size: %d\n", queue.size());
  if (queue.empty()) {
    square = NULL;
  } else {
    square = queue.front();
    queue.pop_front();
  }
  SDL_UnlockMutex(queue_mutex);
  return square;
}

int thread_launcher(void *data) {
  Thread* thread = reinterpret_cast<Thread*>(data);
  return thread->run();
}

Thread::Thread() { running = false; }
void Thread::stop() { running = false; }

int Thread::run() {
  running = true;
  int debug_time = SDL_GetTicks(); // DEBUG
  int calc_count = 0, total_count = 0;

  Square *sq = plotter.get_square(NULL);
  while (sq) {
    float (*pix)[square_size] = sq->pix;
    int step = sq->step;
    int step2 = step * 2;
    int start_x = (sq->fase & 1) ? sq->step : 0;
    int start_y = (sq->fase & 2) ? sq->step : 0;
    int draw_size = (sq->fase == 4) ? step2 : step;
    double max_div = .5 / (step * step * step);

    for (int y = start_y; y < sq->y_max; y += step2) {
      double ci = plotter.y_to_i(y + sq->y_offset);
      for (int x = start_x; x < sq->x_max; x += step2) {
        if (!running) return 0;

        bool do_calc = true;
        if (sq->fase != 4 && x >= step && y >= step &&
            x < sq->x_max - step && y < sq->y_max - step) {

          float nb_pix[4];
          if (sq->fase == 3) {
            float temp[] = {pix[y-step][x-step], pix[y-step][x+step],
                            pix[y+step][x-step], pix[y+step][x+step]};
            memcpy(nb_pix, temp, sizeof(temp));
          } else {
            float temp[] = {pix[y-step][x], pix[y][x-step],
                            pix[y+step][x], pix[y][x+step]};
            memcpy(nb_pix, temp, sizeof(temp));
          }
          if (*std::max_element(nb_pix, nb_pix+4) -
              *std::min_element(nb_pix, nb_pix+4) <= max_div) {
            pix[y][x] = (nb_pix[0] + nb_pix[1] + nb_pix[2] + nb_pix[3]) / 4;
            do_calc = false;
          }
        }
        total_count++;
        if (do_calc) {
          calc_count++;
          pix[y][x] = plotter.iterate(plotter.x_to_r(x + sq->x_offset), ci);
          //pix[y][x].iter = rand() / 100000.;
        }
        Uint32 color = palette.get_color(pix[y][x]);

        gfx.draw_rect(x + sq->x_offset, y + sq->y_offset,
                      draw_size, draw_size, color);
        //if (do_calc) gfx.draw_pixel(x + sq->x_offset, y + sq->y_offset, 0xffffff);
      }
    }
    sq = plotter.get_square(sq);
  }

  int percent = (total_count) ? calc_count * 100 / total_count : 0;
  printf("%d%% of %d pixels calculated in %d ms\n",
         percent, total_count, SDL_GetTicks() - debug_time);

  running = false;
  return 0;
}
