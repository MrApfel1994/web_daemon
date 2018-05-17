
#include <QtWidgets/QApplication>
#include <QtWebKitWidgets/QWebInspector>

#include <QtCore/QtPlugin>
#include <QtNetwork/QNetworkReply>

#include <fstream>
#include <iostream>
#include <thread>

#include "WebApp.h"
#include "WebView.h"

#ifdef __linux__
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QXcbGlxIntegrationPlugin)

void gst_load__default_plugins();
#endif

#ifdef XCB_USE_EGL
#error "qqqq"
#endif

int main(int argc, char *argv[]) {
#ifdef __linux__
    gst_load__default_plugins();
#endif
    
    QApplication::setAttribute(Qt::AA_X11InitThreads);
    
    QApplication qt_app(argc, argv);
    qt_app.setQuitOnLastWindowClosed(true);

    std::string app_id = "0";
    std::string default_url = "https://get.webgl.org";//"https://www.quirksmode.org/html5/tests/video.html";
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
    
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebGLEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, false);

    WebView web_view;
    web_view.setAttribute(Qt::WA_DontShowOnScreen);
    web_view.show();
    web_view.load(QUrl(default_url.c_str()));
    
    QObject::connect(web_view.page()->networkAccessManager(),
                     (void (QNetworkAccessManager::*)(QNetworkReply *, const QList<QSslError> &))&QNetworkAccessManager::sslErrors,
                     [](QNetworkReply *r, const QList<QSslError> &errors){
                         //for (const auto &e : errors) std::cout << e.errorString().toUtf8().constData() << std::endl;
                         r->ignoreSslErrors();
                     });

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
