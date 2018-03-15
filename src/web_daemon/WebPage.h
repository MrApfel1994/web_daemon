#pragma once

#include <QtWebKit/QWebPage>

class WebPage : public QWebPage {
    Q_OBJECT
public:
    explicit WebPage(QObject *parent) : QWebPage(parent) {}

    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID) override;

Q_SIGNALS:
    void JsConsoleMessage(const QString &id, int line, const QString &msg);
};

