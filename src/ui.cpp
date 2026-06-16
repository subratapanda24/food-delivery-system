#include "../include/ui.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[H";
}

void printLine(int len, const string& color) {
    cout << color;
    for (int i = 0; i < len; i++) cout << "-";
    cout << RESET << "\n";
}

void printBox(const string& title, const string& color) {
    (void)color;
    int w = 68;
    int pad = (w - 2 - (int)title.size()) / 2;
    cout << CORAL;
    for (int i = 0; i < w; i++) cout << "=";
    cout << RESET << "\n";
    cout << CORAL << "|" << RESET;
    cout << string(pad, ' ') << BOLD << title << RESET;
    cout << string(w - 2 - pad - (int)title.size(), ' ');
    cout << CORAL << "|" << RESET << "\n";
    cout << CORAL;
    for (int i = 0; i < w; i++) cout << "=";
    cout << RESET << "\n";
}

void printProgressBar(const string& label, double pct, int width, const string& color) {
    (void)color;
    int filled = (int)(pct / 100.0 * width);
    filled = max(0, min(width, filled));
    cout << "  " << left << setw(22) << label << " [";
    for (int i = 0; i < filled; i++) cout << "\xe2\x96\x88";
    for (int i = filled; i < width; i++) cout << " ";
    cout << "] " << BOLD << fixed << setprecision(1) << pct << "%" << RESET << "\n";
}

void printSuccess(const string& msg) {
    cout << "  " << BGREEN << "[OK]" << RESET << " " << msg << "\n";
}

void printError(const string& msg) {
    cout << "  " << BRED << "[ERR]" << RESET << " " << msg << "\n";
}

void printInfo(const string& msg) {
    cout << "  " << CYAN << "[i]" << RESET << " " << msg << "\n";
}

void printWarning(const string& msg) {
    cout << "  " << YELLOW << "[!]" << RESET << " " << msg << "\n";
}

string getTimestamp() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
    return string(buf);
}
