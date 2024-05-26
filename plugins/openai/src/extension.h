// Copyright (C) 2024 Kevin Kim (chaeya@gmail.com)

#pragma once
#include <QLoggingCategory>
#include <QObject>
#include <memory>
#include <QtNetwork/QNetworkAccessManager>  // 추가된 부분
#include "albert/extension.h"
#include "albert/queryhandler.h"
Q_DECLARE_LOGGING_CATEGORY(qlc)

namespace OpenAI {

class Extension final : public Core::Extension, public Core::QueryHandler
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ALBERT_EXTENSION_IID FILE "metadata.json")

public:

    Extension();
    ~Extension();

    QString name() const override { return "OpenAI"; }
    QWidget *widget(QWidget *parent = nullptr) override;
    QStringList triggers() const override { return {"ai>"}; }
    void handleQuery(Core::Query * query) const override;

private:

    class Private;
    std::unique_ptr<Private> d;
    QNetworkAccessManager *networkManager;

};
}

