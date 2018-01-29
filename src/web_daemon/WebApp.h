#pragma once

#include <mutex>
#include <string>
#include <vector>

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include <common/SharedMemory.h>

class WebView;

class WebApp : public QObject {
    Q_OBJECT
public:
    WebApp(const char *app_id, WebView *web_view);
    ~WebApp();

    WebApp(const WebApp &rhs) = delete;
    WebApp &operator=(const WebApp &rhs) = delete;

    int Run();

    void *frame_buf() { return framebuf_mem_.buf(); }

private slots:
    void OnLoadStarted();
    void OnLoadProgress(int progress);
    void OnLoadFinished(bool result);
    void OnTitleChanged(const QString &title);
    void OnUrlChanged(const QUrl &url);

private:
    void ProcessMessage(const void *in_buf, uint32_t in_size, void *out_buf, uint32_t &out_size);

    bool OnResize(int w, int h);

    bool running_;
    std::string app_id_;
    int w_, h_;

    WD::SharedMemory framebuf_mem_;

    WebView *web_view_;

    std::mutex mtx_;
    std::vector<std::vector<uint8_t>> delayed_messages_;
};
