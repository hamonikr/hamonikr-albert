---
layout: docs
title: Troubleshooting
permalink: /docs/faq/
---

##### Why are items not sorted the way I prefer?

Simply because Albert does not know your preferences. The results of the different plugins are not sorted in a special order. Fortunately Albert remembers your usage patterns. In detail Albert sorts by urgency, score and length of title, in this order. The score is determined by the score an extension gives the item and the usage score. The score of the extension is often the ratio of the match length to title length and the usage score is your usage history with a pinch of magic. If you use albert a while you will get a comfortable order of the items. Just "train" it.

##### How can I autostart Albert?

At the moment there is no autostart option, since this application is meant to be desktop agnostic and there is no standard way to achieve this. The most promising approach for the future is the XDG [Desktop Application Autostart Specification](http://standards.freedesktop.org/autostart-spec/autostart-spec-latest.html) but not all desktop environments do support it. Link the desktop file into your autostart directory: `ln -s /usr/share/applications/albert.desktop ~/.config/autostart/`

##### Which application is used to open an item?

Unless explicitly defined by the responsible extension, Albert uses the system defaults. They are managed by your desktop environment and defined as stated in the freedesktop.org [Association between MIME types and applications](http://standards.freedesktop.org/mime-apps-spec/mime-apps-spec-1.0.html) standard. See the [Arch Linux](https://wiki.archlinux.org/index.php/Default_applications#MIME_types_and_desktop_entries) or [Debian](https://wiki.debian.org/MIME) wiki for a human readable version of the standard. One generic way to change those associations is `xdg-mime`. Check the manual for more informations.

##### "FATAL: Stylefile not found: xxx", where is it?

Albert searches for theme files in the directory `albert/themes` in the [QStandardPaths::AppDataLocation](http://doc.qt.io/qt-5/qstandardpaths.html#StandardLocation-enum). Most probably you did not *install* Albert (The installation process places the stylefiles, icons and libraries in the correct places) or you did it in a place where the theme directories are not in an AppDataLocation. This can happen if you use an unusual `CMAKE_INSTALL_PREFIX`.

##### Can I let Albert ignore certain files/folders?

Create a file called `.albertignore` in the folder that contains the file you want to ignore. Fill it with the filenames you want to ignore. One per line. The ignores supports [wildcard matching](http://doc.qt.io/qt-5/qregexp.html#wildcard-matching). Global ignores like e.g. the `.gitignore` provides are currently not supported.

###### Error. Key-XYZ could not be registered.

This is what Albert tells you, if the window system refused to register the key combination¹. This may have many reasons, but the most prominent is that another application already grabbed the key combo. If you really want to get exactly that combo, your best bet is to find out which application grabbed the key and disable the grab. In virtually every case this will be your desktop environment, respectively its window manager, e.g. Compiz (Unity), KWin (KDE), Mutter (Gnome), or Muffin (Cinnamon). If it is possible remove the desired key combination from the system's keyboard settings and try again to set it Albert. If the grab is hardcoded you're out of luck. Use the workaround below or use another key.

**¹** <span style="font-size: 12px">Actually a registration of one key combination is made up of 4 grabs. Since X11 considers numlock and capslock as modifiers, the actual grabs comprise every permutation of num- and capslock modifiers plus the actual key combo. If one grab fails the registration is considered as failed.</span>

###### Hotkey is set but does not work

This is one of the weirdest and most annoying issue on linux. There may be several reasons. Known so far are:

* Wayland does not allow to grab keys at all. Magically the hotkey works on X11 windows but not on wayland windows. 
* Sometimes crappy window managers (e.g. Unity) intercept the keypress event. The event is simply not delivered.

For these use the workaround described below.

###### General workaround for hotkey problems

The workaround for the problems above is to let the desktop environment handle the registration of hotkeys and run the command `albert show` or `albert toggle` on activation. This implies that a process has to be run and is somewhat heavy. Try to get native hotkeys first.

##### Why are my icons are not displayed correctly?

Make sure you have libqt5-svg (May be slightly different on some distributions) installed. Further make sure that Qt has the correct icon theme set. This is not a problem of Albert in particular, but of all Qt applications. This is a common problem and the internet provides solutions to the tons of possible reasons. Two mainstream options: Get Qt to [inherit the GTK icon theme](https://wiki.archlinux.org/index.php/Uniform_look_for_Qt_and_GTK_applications#Using_a_GTK.2B_icon_theme_in_Qt_apps) or use qt5ct to [configure Qt](https://wiki.archlinux.org/index.php/qt#Configuration_of_Qt5_apps_under_environments_other_than_KDE_Plasma) (not recommended, qt5ct messes up other things).

##### Why are some applications not shown?

The [Desktop Entry Specification](https://specifications.freedesktop.org/desktop-entry-spec/latest/) defines which applications shall be shown. If the desktop entry contains the key value pair `NoDisplay=true`, the application will not be shown. Further the keys `OnlyShowIn` and `NotShowIn` define which desktop environments should (not) show the desktop entry. The values are compared to the environment variable `$XDG_CURRENT_DESKTOP`.

##### .\* Wayland .\* ?

Sorry, I can't help you with this. Waylands integration into the major DEs is far from mature. Wayland support needs a lot of time and hopefully these infancy problems vanish with time. I'd like to invest the little time I have to do things [essential](https://en.wikipedia.org/wiki/No_Silver_Bullet) to the development of albert. If you google a bit, you will find out that a lot of applications have problems with Wayland. However if you are willing I'd appreciate to get help with that problem.

If you came here because the hotkey does not work under Wayland see the section on hotkeys above.

##### HiDPI?
 
Unfortunately Qt does not support [HiDPI scaling](http://doc.qt.io/qt-5/highdpi.html) until 5.6. From there on you can use the environment variable `QT_SCALE_FACTOR=2` to run albert scaled. Further you can use `QT_AUTO_SCREEN_SCALE_FACTOR=1` to enable automatic scaling, based on the pixel density of the monitor.

Note that v0.14.9 introduces automated scaling based on the dpi the screen reports. The scale factors are multiplicative. Setting QT_SCALE_FACTOR=2 on a 2x device gives an effective devicePixelRatio of 4. v0.14.13 changes this behavior when environment variables related to scaling are set, no automatic scaling is applied.

The automatic scaling may not fit your needs, especially on small HiDPI screens people tend to rather want small scaling. If you are not satisfied with the automatic scaling Qt applies, just set one of the environment variables, e.g. `QT_SCALE_FACTOR=1`.

##### fatal error: \*.h: No such file or directory.

Check if you have installed the dependencies. A good way to check the current dependencies is to check `depends`, `makedepends` and `optdepends` sections in the official Arch Linux [PKGBUILD](https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=albert). If you don't want to build a specific plugin you don't have to. Configure CMake accordingly by setting build switches, e.g. `-DBUILD_VIRTUALBOX=OFF`. See the possible build switches and their default values [here](https://github.com/albertlauncher/plugins/blob/master/CMakeLists.txt).

In general you should avoid compiling albert on your own and use package managers.

##### Seems like I dont use the official repos

You have to be one of every second person that got albert from **unofficial** sources somewhere in the internet. This _sucks_ because 1st **you** are at risk to run malicious code on your machine and 2nd **I** am made responsible for whatever is not working. Please use the official repositories hosted at OpenSuse Build System to be safe and up to date. If something is not working, report the problem instead of using other sources. It's really as easy as that:

* Trust the key on your machine.
* Add the repository to your package manager.
* Install albert and update your system every now and then.

This process is also explained a bit more elaborate in the [docs](https://albertlauncher.github.io/docs/installing/#using-package-managers).

##### I get a black border/background, i.e. no transparency

Either you dont have hardware graphics acceleration or you are missing a [compositor](https://en.wikipedia.org/wiki/Compositing_window_manager). These problems are not related to albert, but are a general desktop misconfiguration. Google will help you to solve it. If there are other reasons for this symptom, please let me know.

##### Launcher appears behind maximized windows

Some window managers prioritize maximized windows over popups. The suggested way to fix this is to find an "Always on Top" option in your window manager. If such a thing doesn't exist however you can use [`wmctrl`](http://tripie.sweb.cz/utils/wmctrl/) to manually raise it by mapping a hotkey to the following command:

```
albert toggle && wmctrl -a "albert — Albert"
```
