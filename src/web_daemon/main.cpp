
#include <QtWidgets/QApplication>
#include <QtWebKitWidgets/QWebInspector>

#include <fstream>
#include <iostream>
#include <thread>

#include "WebApp.h"
#include "WebView.h"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_X11InitThreads);

    QApplication qt_app(argc, argv);
    qt_app.setQuitOnLastWindowClosed(true);

    std::string app_id = "0";
    std::string default_url = "http://html5test.com";
    uint32_t parent_proc_id = 0;

    if (argc > 1) {
        app_id = argv[1];
        if (argc > 2) {
            default_url = argv[2];
            if (argc > 3) {
                parent_proc_id = (uint32_t)atoi(argv[3]);
            }
        }
    }

    WebView web_view;
    web_view.setAttribute(Qt::WA_DontShowOnScreen);
    web_view.show();
    web_view.load(QUrl(default_url.c_str()));

    /*web_view.page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    QWebInspector inspector;
    inspector.setPage(web_view.page());
    inspector.setVisible(true);*/

#if defined(QT_OPENSSL) && !defined(QT_NO_OPENSSL)
    web_view.connect(web_view.page()->networkAccessManager(),
                     SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)),
                     SLOT(sslErrorHandler(QNetworkReply*, const QList<QSslError> &)));
#endif

    auto log_file = std::ofstream{ app_id + "_log.txt", std::ios::binary };

    WebApp web_app(app_id.c_str(), &web_view, parent_proc_id, log_file);

    auto thr = std::thread{[&web_app]() {
        web_app.Run();
        QApplication::closeAllWindows();
    }};

    int ret = qt_app.exec();

    log_file << "Joining thread" << std::endl;
    thr.join();

    return ret;
}