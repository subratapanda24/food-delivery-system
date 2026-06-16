#pragma once
#include <string>

#define RESET     "\033[0m"
#define BOLD      "\033[1m"
#define DIM       "\033[2m"

#define RED       "\033[31m"
#define GREEN     "\033[32m"
#define YELLOW    "\033[33m"
#define BLUE      "\033[34m"
#define CYAN      "\033[36m"
#define WHITE     "\033[37m"

#define BRED      "\033[91m"
#define BGREEN    "\033[92m"
#define BYELLOW   "\033[93m"
#define BCYAN     "\033[96m"
#define BWHITE    "\033[97m"

#define ORANGE    "\033[38;5;208m"

#define CORAL     "\033[38;5;203m"

#define BG_BLACK  "\033[40m"
#define BG_RED    "\033[41m"
#define BG_GREEN  "\033[42m"
#define BG_BLUE   "\033[44m"
#define BG_CYAN   "\033[46m"
#define BLACK     "\033[30m"

void clearScreen();
void printLine(int len = 68, const std::string& color = DIM);
void printBox(const std::string& title, const std::string& color = BWHITE);
void printProgressBar(const std::string& label, double pct, int width = 30,
                      const std::string& color = BGREEN);
void printSuccess(const std::string& msg);
void printError(const std::string& msg);
void printInfo(const std::string& msg);
void printWarning(const std::string& msg);
std::string getTimestamp();
