//Author: Francesco Cirotto
// This class provides default debugging message (in different color)

#include "colormod.h"
#include <iostream>

#ifndef DEBUG

#define DEBUG(lvl, msg, color) do {						\
    Color::Modifier def(Color::FG_DEFAULT);				\
    Color::Modifier bold(Color::BOLD);				\
    Color::Modifier rsbold(Color::RESETBOLD);				\
    Color::Modifier def_bg(Color::BG_DEFAULT);				\
    Color::Modifier col(color);					\
    Color::Modifier rsall(Color::RESETALL);					\
    std::cerr << col << bold << lvl << "\t"<< __PRETTY_FUNCTION__ <<"\t" << msg << rsall << std::endl; \
} while (0)

#define LOG(x) DEBUG("LOG", x, Color::FG_BLUE )
#define ERROR(x) DEBUG("ERROR", x, Color::FG_RED)
#define WARNING(x) DEBUG("WARNING",x, Color::FG_YELLOW)
#define INFO(x) DEBUG("INFO", x, Color::FG_CYAN)
#define SUCCESS(x) DEBUG("SUCCESS", x, Color::FG_GREEN)

#endif
