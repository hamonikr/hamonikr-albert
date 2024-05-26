// Copyright (C) 2014-2018 Manuel Schneider

#pragma once
#include "ui_configwidget.h"

namespace QmlBoxModel {

class MainWindow;

class ConfigWidget final : public QWidget
{
    Q_OBJECT

public:

    ConfigWidget(MainWindow *mainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = {});

private:

    void onStyleChanged(int);
    void onThemeChanged(const QString &text);
    void updateThemes();

    MainWindow *mainWindow_;
    Ui::SettingsDialog ui;
};

}
