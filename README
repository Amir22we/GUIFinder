
# GUI Finder (`guif`)

`guif` is a simple command-line utility for Linux/X11 that lists all GUI windows currently open in your session.

It requires no arguments or filters: just run it and see all running graphical applications.

---

## Features

* Lists all GUI windows of the current user.
* Only X11.
* Easy to compile and install.
* Works with any modern window manager supporting EWMH (KDE, GNOME, XFCE, i3, etc.).

---

## Installation

1. Make sure you have the required libraries installed:

```bash
sudo pacman -S libx11
```

2. Download the `main.cpp` source file.

3. Compile the binary:

```bash
g++ main.cpp -lX11 -o guif
```

Now you can run `guif` from anywhere.

---

## Usage

Simply run:

```bash
./guif
```

Or, if installed globally:

```bash
guif
```

Example output:

```
Firefox
Telegram
VS Code
Terminal
```

Each line represents the title of a GUI window.

---

## Launching `guif` via an Icon 

You can create a desktop launcher so that `guif` can be started from your application menu or by clicking an icon.

1. Create a `.desktop` file:

```bash
nano ~/.local/share/applications/guif.desktop
```

2. Paste the following content (adjust your terminal emulator if needed):

```ini
[Desktop Entry]
Name=guif
Comment=List all X11 GUI windows
Exec=gnome-terminal -- bash -c "guif; exec bash"
Icon=utilities-terminal
Terminal=false
Type=Application
Categories=System;Utility;
```

* Replace `gnome-terminal` with your terminal emulator (`xfce4-terminal`, `konsole`, `alacritty`, etc.) if needed.
* `Icon` - optional; you can set your own icon path.

1. Make the `.desktop` file executable:

```bash
chmod +x ~/.local/share/applications/guif.desktop
```

4. Refresh your desktop environment's application menu (log out/in, or run `update-desktop-database` if needed).

Now `guif` will appear in your menu, and clicking it will open a terminal showing all GUI windows.

---

## Notes

* The utility does not filter windows and does not show non-GUI processes.
* Requires X server (works on X11).
* Your window manager must support EWMH.

---

## License

MIT License.

---
