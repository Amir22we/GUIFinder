#include <X11/Xlib.h> // X сервер
#include <X11/Xatom.h> // для айди окон
#include <iostream>
#include <string>
using namespace std;

// получаем название окон
string getWindowName(Display* d, Window w) { // Display* d для подключения к X серверу и window W для получения айди окна x11
    Atom prop = XInternAtom(d, "_NET_WM_NAME", False); // Получение имени окна
    Atom utf8 = XInternAtom(d, "UTF8_STRING", False); // чтение utf8

    Atom actualType; // тип окна
    int actualFormat; // формат окна
    unsigned long nItems, bytesAfter; // количество байт и свойств
    unsigned char* propData = nullptr; // укащатели данных

    if (XGetWindowProperty(d, w, prop, 0, (~0L), False, utf8, &actualType, &actualFormat, &nItems, &bytesAfter, &propData) == Success && propData) {
        string name((char*)propData); // propdata в стринг
        XFree(propData); // освобождение памяти
        return name;
    }
    return ""; // в случае else
}

int main() {
    Display* d = XOpenDisplay(nullptr); 
    if (!d) {
        cerr << "Не смог открыть X дисплей\n";
        return 1;
    }

    Window root = DefaultRootWindow(d);
    Atom clientList = XInternAtom(d, "_NET_CLIENT_LIST", True); // получаем корневое окно
    if (clientList == None) {
        cerr << "WM не поддерживает EWMH\n";
        return 1;
    }

    Atom actualType;
    int actualFormat;
    unsigned long nItems, bytesAfter;
    unsigned char* data = nullptr;

    if (XGetWindowProperty(d, root, clientList, 0, (~0L), False, AnyPropertyType, &actualType, &actualFormat, &nItems, &bytesAfter, &data) != Success || !data) {
        cerr << "Не удалось получить список окон\n";
        return 1;
    }

    Window* windows = (Window*)data; // data в массив Window*
    for (unsigned long i = 0; i < nItems; i++) {
        string name = getWindowName(d, windows[i]); // получаем имена
        if (!name.empty()) {
            cout << name << '\n';
        }
    }

    XFree(data);
    XCloseDisplay(d); // закрытие сервера 
    return 0;
}
