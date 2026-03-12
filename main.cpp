#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// ── ANSI цвета ──────────────────────────────────────────────────────────────
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

#define FG_WHITE    "\033[97m"
#define FG_CYAN     "\033[96m"
#define FG_YELLOW   "\033[93m"
#define FG_BLUE     "\033[34m"
#define FG_GREEN    "\033[92m"
#define FG_MAGENTA  "\033[95m"
#define FG_GRAY     "\033[90m"

#define BG_DARK     "\033[48;5;235m"
// ────────────────────────────────────────────────────────────────────────────

// получаем название окна
string getWindowName(Display* d, Window w) {
    Atom prop = XInternAtom(d, "_NET_WM_NAME", False);
    Atom utf8 = XInternAtom(d, "UTF8_STRING", False);

    Atom actualType;
    int actualFormat;
    unsigned long nItems, bytesAfter;
    unsigned char* propData = nullptr;

    if (XGetWindowProperty(d, w, prop, 0, (~0L), False, utf8,
        &actualType, &actualFormat, &nItems, &bytesAfter, &propData) == Success && propData) {
        string name((char*)propData);
        XFree(propData);
        return name;
    }
    return "";
}

// количество видимых символов (без escape-кодов) для выравнивания
size_t visibleLen(const string& s) {
    size_t len = 0;
    bool esc = false;
    for (char c : s) {
        if (c == '\033') { esc = true; continue; }
        if (esc) { if (c == 'm') esc = false; continue; }
        len++;
    }
    return len;
}

// повтор UTF-8 строки n раз
string repeat(const string& s, int n) {
    string result;
    result.reserve(s.size() * n);
    for (int i = 0; i < n; i++) result += s;
    return result;
}

void printHeader(int width) {
    const string title = "  \u25c8 X11 Window List ";
    int pad = width - (int)title.size() - 2;

    cout << FG_CYAN << BOLD
         << "\u2554" << repeat("\u2550", width) << "\u2557\n"
         << "\u2551" << title << string(pad, ' ') << "\u2551\n"
         << "\u2560" << repeat("\u2550", width) << "\u2563\n"
         << RESET;
}

void printFooter(int count, int width) {
    string info = "  " + to_string(count) + " \u043e\u043a\u043e\u043d \u043d\u0430\u0439\u0434\u0435\u043d\u043e";
    int pad = width - (int)info.size() - 2;

    cout << FG_CYAN << BOLD
         << "\u2560" << repeat("\u2550", width) << "\u2563\n"
         << "\u2551" << FG_YELLOW << info << FG_CYAN << string(pad, ' ') << "\u2551\n"
         << "\u255a" << repeat("\u2550", width) << "\u255d\n"
         << RESET;
}

void printRow(int index, const string& name, int width, bool odd) {
    // цвет строки: чётные/нечётные чередуются
    const char* rowColor = odd ? FG_WHITE : FG_GRAY;

    // номер с отступом
    string num = to_string(index + 1);
    string numPad = string(3 - num.size(), ' ') + num;

    // обрезаем имя если слишком длинное
    string displayName = name;
    int maxName = width - 8; // 3 (num) + 3 (gaps) + 2 (borders)
    if ((int)displayName.size() > maxName)
        displayName = displayName.substr(0, maxName - 1) + "…";

    int pad = width - 6 - (int)displayName.size();

    cout << FG_CYAN << BOLD << "\u2551" << RESET
         << FG_MAGENTA << " " << numPad << " " << RESET
         << FG_BLUE << "\u2502" << RESET
         << rowColor << " " << displayName << string(pad, ' ') << RESET
         << FG_CYAN << BOLD << "\u2551\n" << RESET;
}

int main() {
    Display* d = XOpenDisplay(nullptr);
    if (!d) {
        cerr << BOLD << FG_YELLOW << "✗ " << RESET
             << "Не смог открыть X дисплей\n";
        return 1;
    }

    Window root = DefaultRootWindow(d);
    Atom clientList = XInternAtom(d, "_NET_CLIENT_LIST", True);
    if (clientList == None) {
        cerr << BOLD << FG_YELLOW << "✗ " << RESET
             << "WM не поддерживает EWMH\n";
        return 1;
    }

    Atom actualType;
    int actualFormat;
    unsigned long nItems, bytesAfter;
    unsigned char* data = nullptr;

    if (XGetWindowProperty(d, root, clientList, 0, (~0L), False, AnyPropertyType,
        &actualType, &actualFormat, &nItems, &bytesAfter, &data) != Success || !data) {
        cerr << BOLD << FG_YELLOW << "✗ " << RESET
             << "Не удалось получить список окон\n";
        return 1;
    }

    Window* windows = (Window*)data;

    // собираем имена
    vector<string> names;
    for (unsigned long i = 0; i < nItems; i++) {
        string name = getWindowName(d, windows[i]);
        if (!name.empty()) names.push_back(name);
    }
    XFree(data);
    XCloseDisplay(d);

    // вычисляем ширину по самому длинному имени
    int maxLen = 40;
    for (auto& n : names)
        maxLen = max(maxLen, (int)n.size());
    maxLen = min(maxLen, 80); // ограничиваем до 80 символов
    int width = maxLen + 8;   // отступы + номер + разделитель

    cout << "\n";
    printHeader(width);

    for (int i = 0; i < (int)names.size(); i++) {
        printRow(i, names[i], width, i % 2 == 0);
    }

    printFooter((int)names.size(), width);
    cout << "\n";

    return 0;
}