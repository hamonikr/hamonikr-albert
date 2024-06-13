// Copyright (c) 2014-2022 Manuel Schneider

#pragma once
#include "ui_settingswindow.h"
#include <QWidget>
#include <memory>
class App;
class PluginsWidget;
class HandlerWidget;
class QueryWidget;

class SettingsWindow final : public QWidget
{
    Q_OBJECT
public:
    SettingsWindow(App &app);
    ~SettingsWindow();

    void bringToFront(const QString & = {});

private:
    void init_tab_general_hotkey(App &app);
    void init_tab_general_trayIcon(App &app);
    void init_tab_general_frontends(App &app);
    void init_tab_general_terminals(App &app);
    void insert_tab_about();

    void keyPressEvent(QKeyEvent * event) override;

    Ui::SettingsWindow ui;
    std::unique_ptr<PluginsWidget> plugin_widget;
    std::unique_ptr<QueryWidget> query_widget;
};
