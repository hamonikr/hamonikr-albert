// Copyright (c) 2022-2023 Manuel Schneider

#include "albert/albert.h"
#include "albert/logging.h"
#include "trayicon.h"
#include "albert/util/iconprovider.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>

namespace {
    const char* CFG_SHOWTRAY = "showTray";
    const bool  DEF_SHOWTRAY = true;
}

TrayIcon::TrayIcon() {

    if (!supportsMessages())
        WARN << "Desktop notifications are not supported on this system";

    // https://bugreports.qt.io/browse/QTBUG-53550
    albert::IconProvider ip;
#if defined(Q_OS_MAC)
    QIcon icon(":app_tray_icon");
#else
    QSize size;
    QIcon icon(ip.getPixmap({"xdg:albert-tray", "xdg:albert", "qrc:app_tray_icon"}, &size, QSize(64, 64)));
#endif
    icon.setIsMask(true);
    setIcon(icon);

    setVisible(albert::settings()->value(CFG_SHOWTRAY, DEF_SHOWTRAY).toBool());

    auto *action = menu.addAction(tr("Show/Hide"));
    QObject::connect(action, &QAction::triggered, [](){ albert::toggle(); });

    action = menu.addAction(tr("Settings"));
    QObject::connect(action, &QAction::triggered, [](){ albert::showSettings(); });

    action = menu.addAction(tr("Open website"));
    QObject::connect(action, &QAction::triggered, [](){ albert::openWebsite(); });

    menu.addSeparator();

    action = menu.addAction(tr("Restart"));
    QObject::connect(action, &QAction::triggered, [](){ albert::restart(); });

    action = menu.addAction(tr("Quit"));
    QObject::connect(action, &QAction::triggered, [](){ albert::quit(); });

    setContextMenu(&menu);
}

void TrayIcon::setVisible(bool enable) {
    albert::settings()->setValue(CFG_SHOWTRAY, enable);
    QSystemTrayIcon::setVisible(enable);
}

