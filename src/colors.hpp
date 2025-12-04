#pragma once
#include <string>

namespace colors {

#ifdef _WIN32
#include <windows.h>

inline void enable_colors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
inline void enable_colors() {}
#endif

// Reset
const std::string RESET = "\033[0m";

// Regular colors
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

// Bold
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";

// Bold colors
const std::string BOLD_GREEN = "\033[1;32m";
const std::string BOLD_YELLOW = "\033[1;33m";
const std::string BOLD_CYAN = "\033[1;36m";
const std::string BOLD_WHITE = "\033[1;37m";
const std::string BOLD_BLUE = "\033[1;34m";

// Helper functions
inline std::string red(const std::string& s) { return RED + s + RESET; }
inline std::string green(const std::string& s) { return GREEN + s + RESET; }
inline std::string yellow(const std::string& s) { return YELLOW + s + RESET; }
inline std::string blue(const std::string& s) { return BLUE + s + RESET; }
inline std::string cyan(const std::string& s) { return CYAN + s + RESET; }
inline std::string white(const std::string& s) { return WHITE + s + RESET; }
inline std::string dim(const std::string& s) { return DIM + s + RESET; }
inline std::string bold(const std::string& s) { return BOLD + s + RESET; }
inline std::string bold_green(const std::string& s) { return BOLD_GREEN + s + RESET; }
inline std::string bold_cyan(const std::string& s) { return BOLD_CYAN + s + RESET; }
inline std::string bold_yellow(const std::string& s) { return BOLD_YELLOW + s + RESET; }
inline std::string bold_blue(const std::string& s) { return BOLD_BLUE + s + RESET; }
inline std::string bold_white(const std::string& s) { return BOLD_WHITE + s + RESET; }

} // namespace colors
