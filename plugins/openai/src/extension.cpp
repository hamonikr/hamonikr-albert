// Copyright (C) 2024 Kevin Kim (chaeya@gmail.com)

#include <QDirIterator>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QFuture>
#include <QFutureWatcher>
#include <QPointer>
#include <QProcess>
#include <QStringList>
#include <QtConcurrent>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QClipboard>
#include <QGuiApplication>
#include <QEventLoop>
#include <QThread>
#include <set>
#include <functional>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include "albert/util/standardactions.h"
#include "albert/util/standarditem.h"
#include "extension.h"
#include "xdg/iconlookup.h"
Q_LOGGING_CATEGORY(qlc, "openai")
#define DEBG qCDebug(qlc).noquote()
#define INFO qCInfo(qlc).noquote()
#define WARN qCWarning(qlc).noquote()
#define CRIT qCCritical(qlc).noquote()
using namespace std;
using namespace Core;

class OpenAI::Extension::Private
{
public:
    QPointer<QWidget> widget;
    QString iconPath;

    QString queryOpenAI(const QString &queryString) {
        QNetworkAccessManager networkManager;
        QNetworkRequest request(QUrl("https://api.openai.com/v1/completions"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("Authorization", "Bearer OPENAI_API_KEY");

        QJsonObject json;
        json["model"] = "gpt-3.5-turbo";
        json["prompt"] = queryString;
        json["max_tokens"] = 100;

        QNetworkReply *reply = networkManager.post(request, QJsonDocument(json).toJson());
        QEventLoop eventLoop;
        QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();

        QString answer;
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll());
            qDebug() << "API Response:" << response;  // 응답 본문 출력
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            answer = jsonResponse["choices"][0]["text"].toString().trimmed();
        } else {
            answer = "Error: " + reply->errorString();
        }

        reply->deleteLater();
        return answer;
    }

};

OpenAI::Extension::Extension() : Core::Extension("org.albert.extension.openai"),
                                 Core::QueryHandler(Core::Plugin::id()),
                                 d(new Private) {
    registerQueryHandler(this);

    d->iconPath = ":/openai";  // 변경된 부분

    if (d->iconPath.isNull()) {
        d->iconPath = ":/resources/openai.svg";  // 리소스 파일에서 아이콘 경로 설정
    }
}

OpenAI::Extension::~Extension() {
}

QWidget *OpenAI::Extension::widget(QWidget *parent) {
    if (d->widget.isNull()) {
        d->widget = new QWidget(parent);
        QVBoxLayout *verticalLayout = new QVBoxLayout();
        d->widget->setLayout(verticalLayout);

        QLabel *label = new QLabel();
        label->setWordWrap(true);
        label->setText(QCoreApplication::translate("OpenAI::ConfigWidget",
            "The OpenAI extension allows you to ask questions and get answers "
            "using the OpenAI API. Just invoke the extension using the trigger 'ai>'.", nullptr));
        verticalLayout->addWidget(label);

        QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        verticalLayout->addItem(verticalSpacer);
    }
    return d->widget;
}

void OpenAI::Extension::handleQuery(Core::Query *query) const {
    if (!query->isTriggered() || query->string().trimmed().isEmpty())
        return;

    QString queryString = query->string().trimmed();
    QString answer = d->queryOpenAI(queryString);
    auto item = makeStdItem(QString(),
        d->iconPath, queryString, QString("OpenAI response: %1").arg(answer),
        ActionList {
            std::make_shared<ClipAction>("Copy response", answer)
        }
    );
    query->addMatch(item, 0);
}
