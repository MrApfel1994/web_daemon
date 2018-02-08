
#include <QtGui/QApplication>
//#include <QtNetwork/QsslConfiguration>
#include <QtNetwork/QNetworkReply>

#include <fstream>
#include <iostream>
#include <thread>

#include "WebApp.h"
#include "WebView.h"

void sslErrorHandler(QNetworkReply* qnr, const QList<QSslError> & errlist) {
    qnr->ignoreSslErrors();
}

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_X11InitThreads);

    QApplication qt_app(argc, argv);

    qt_app.setQuitOnLastWindowClosed(false);

    {
        //QSslConfiguration sslconf = QSslConfiguration::defaultConfiguration();
        //QList<QSslCertificate> cert_list = sslconf.caCertificates();
    }


    std::string app_id = "0";
    std::string default_url = "http://html5test.com";

    if (argc > 1) {
        app_id = argv[1];
        if (argc > 2) {
            default_url = argv[2];
        }
    }

    WebView web_view;
    web_view.setAttribute(Qt::WA_DontShowOnScreen);
    web_view.show();
    web_view.load(QUrl(default_url.c_str()));

#if defined(QT_OPENSSL) && !defined(QT_NO_OPENSSL)
    web_view.connect(web_view.page()->networkAccessManager(),
                     SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)),
                     SLOT(sslErrorHandler(QNetworkReply*, const QList<QSslError> &)));
#endif

    auto log_file = std::ofstream{ app_id + "_log.txt", std::ios::binary };

    WebApp web_app(app_id.c_str(), &web_view, log_file);

    auto thr = std::thread{[&web_app]() {
        return web_app.Run();
    }};

    int ret = qt_app.exec();

    log_file << "Joining thread" << std::endl;
    thr.join();

    return ret;
}