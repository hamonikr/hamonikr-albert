// QPointer를 제거하고 일반 포인터를 사용합니다.
// 타이머를 직접 관리하여 올바른 스레드에서 동작하도록 합니다.

#include <QDirIterator>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QFuture>
#include <QFutureWatcher>
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
#include <QSettings>
#include <QTimer>
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

namespace OpenAI {

class Extension::Private {
public:
    QWidget *widget;
    QString iconPath;
    QString apiKey;
    QTimer *queryTimer;
    QString pendingQuery;
    Core::Query *currentQuery;
    std::shared_ptr<StandardItem> placeholderItem;

    Private();
    QString queryOpenAI(const QString &queryString, QString &error);
};

Extension::Private::Private() : queryTimer(new QTimer()), currentQuery(nullptr) {
    QSettings settings(QDir::homePath() + "/.config/albert/albert.conf", QSettings::IniFormat);
    apiKey = settings.value("org.albert.extension.openai/api_key", "").toString();
    if (apiKey.isEmpty()) {
        WARN << "API key is not set in the configuration file.";
    } else {
        INFO << "API key loaded successfully.";
    }

    queryTimer->setSingleShot(true);
    queryTimer->moveToThread(QCoreApplication::instance()->thread()); // 타이머를 메인 스레드로 이동
}

QString Extension::Private::queryOpenAI(const QString &queryString, QString &error) {
    DEBG << "Starting queryOpenAI with query string:" << queryString;

    if (apiKey.isEmpty()) {
        error = "Error: API key is not set.";
        return "";
    }

    QNetworkAccessManager networkManager;
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());

    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";
    QJsonArray messages;
    QJsonObject message;
    message["role"] = "user";
    message["content"] = queryString;
    messages.append(message);
    json["messages"] = messages;
    json["max_tokens"] = 100;

    QNetworkReply *reply = networkManager.post(request, QJsonDocument(json).toJson());
    DEBG << "Network request sent, waiting for reply...";

    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QString answer;
    if (reply->error() == QNetworkReply::NoError) {
        QString response = QString::fromUtf8(reply->readAll());
        DEBG << "API Response received:" << response;  // 응답 본문 출력
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
        answer = jsonResponse["choices"][0]["message"]["content"].toString().trimmed();
        DEBG << "Parsed answer:" << answer;
    } else {
        error = "Error: " + reply->errorString();
        DEBG << "Error occurred:" << error;
    }

    reply->deleteLater();
    return answer;
}

Extension::Extension() : Core::Extension("org.albert.extension.openai"),
                         Core::QueryHandler(Core::Plugin::id()),
                         d(new Private) {
    DEBG << "OpenAI Extension constructor called.";
    registerQueryHandler(this);

    d->iconPath = ":/openai";  // 변경된 부분

    if (d->iconPath.isNull()) {
        d->iconPath = ":/resources/openai.svg";  // 리소스 파일에서 아이콘 경로 설정
    }

    connect(d->queryTimer, &QTimer::timeout, this, [this]() {
        if (d->currentQuery) {
            QString error;
            QString answer = d->queryOpenAI(d->pendingQuery, error);
            DEBG << "Query answered with:" << (error.isEmpty() ? answer : error);

            auto item = makeStdItem(QString(),
                d->iconPath, d->pendingQuery, QString("OpenAI response: %1").arg(error.isEmpty() ? answer : error),
                ActionList {
                    std::make_shared<ClipAction>("Copy response", error.isEmpty() ? answer : error)
                }
            );

            d->currentQuery->addMatch(item, 0);  // query에 매치 추가
            d->currentQuery = nullptr;  // currentQuery를 초기화하여 다시 사용 가능하도록 함
        }
    });
}

Extension::~Extension() {
    DEBG << "OpenAI Extension destructor called.";
}

QWidget *Extension::widget(QWidget *parent) {
    DEBG << "OpenAI widget function called.";
    if (d->widget == nullptr) {
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

void Extension::handleQuery(Core::Query *query) const {
    DEBG << "Handling query with string:" << query->string();
    if (!query->isTriggered() || query->string().trimmed().isEmpty()) {
        DEBG << "Query not triggered or empty.";
        return;
    }

    QString queryString = query->string().trimmed();
    if (queryString.length() < 3) {
        DEBG << "Query string is too short.";
        return;
    }

    d->pendingQuery = queryString;
    d->currentQuery = query;  // 현재 query를 저장

    // Placeholder item to show loading state
    d->placeholderItem = makeStdItem(QString(),
        d->iconPath, queryString, "Waiting for OpenAI response...",
        ActionList {
            std::make_shared<ClipAction>("Copy query", queryString)
        }
    );
    query->addMatch(d->placeholderItem, 0);

    QMetaObject::invokeMethod(d->queryTimer, "start", Q_ARG(int, 1000));  // 1000ms (1초) 후에 타이머가 동작하여 API 호출
}

} // namespace OpenAI

#include "extension.moc"
