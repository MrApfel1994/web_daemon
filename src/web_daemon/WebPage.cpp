#include "WebPage.h"

#include <QtCore/QString>

#include <iostream>

#include "moc_WebPage.cpp"

void WebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID) {
    emit JsConsoleMessage(sourceID, lineNumber, message);

    QWebPage::javaScriptConsoleMessage(message, lineNumber, sourceID);
}