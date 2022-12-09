//Author: Francesco Cirotto
//Namespace with color definition for printing out


#ifndef _COLORMOD_
#define _COLORMOD_

#include <ostream>
namespace Color {
    enum Code {
        FG_RED          = 31,
        FG_GREEN        = 32,
	FG_YELLOW       = 33,
        FG_BLUE         = 34,
	FG_CYAN         = 36,
        FG_DEFAULT      = 39,
        BG_RED          = 41,
        BG_GREEN        = 42,
        BG_BLUE         = 44,
        BG_DEFAULT      = 49,
	BOLD            = 1,
	RESETBOLD       = 21,
	BLINK           = 5,
	RESETBLINK      = 25,
	UNDERLINED      = 4,
	RESETUNDERLINED = 24,
	RESETALL        = 0
	

    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}


#endif
