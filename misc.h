#ifndef TYPES_H
#define TYPES_H

#include <deque>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <fstream>
#include "SDL/SDL.h"

#define LOG_DEBUG() std::cerr << "D: " << __FILE__ << "(" << __LINE__ << ") "
#define LOG_INFO() std::cerr << "I: "
#define LOG_SEPERATOR() std::cerr << "\n\n" << std::flush
#define DEBUG(x) LOG_DEBUG() << #x << " = " << x << std::endl

#endif
