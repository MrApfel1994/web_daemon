#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>

#include <QtCore/QThread>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QLayout>
#include <QtWebKitWidgets/QWebView>

class WebView : public QWebView {
    Q_OBJECT
public:
    void paintEvent(QPaintEvent *ev) override;

    void SetFrameRequested(bool b) {
        /*{   std::lock_guard<std::mutex> lk(mtx_);
            frame_requested_ = b;
        }
        cv_.notify_all();*/
    }

    Q_INVOKABLE bool ProcessEvent(QEvent *ev) {
        bool res = this->event(ev);
        delete ev;
        return true;
    }
    Q_INVOKABLE void Resize(int w, int h, void *new_framebuf) {
        p_framebuf_ = new_framebuf;
        if (w != 0 && h != 0) {
            this->resize(w, h);
        }
    }
    Q_INVOKABLE void LoadUrl(QUrl url) {
        this->load(url);
    }
    Q_INVOKABLE void PageBackward() {
        this->back();
    }
    Q_INVOKABLE void PageForward() {
        this->forward();
    }
    Q_INVOKABLE void PageReload() {
        this->reload();
    }
    Q_INVOKABLE void ChangeZoomFactor(float fac) {
        this->setZoomFactor(fac);
    }

    Q_INVOKABLE void ProcessDelayedRegions();

    std::vector<QRegion> StealUpdatedRegions() {
        return std::move(updated_regions_);
    }

    QWebPage* page() const;

    void load(const QUrl& url);
private:
    void *p_framebuf_ = nullptr;
    QPoint prev_scroll_pos_;
    bool frame_requested_ = true;
    std::condition_variable cv_;

    std::vector<QRegion> updated_regions_;
    std::vector<QRegion> delayed_regions_;

    mutable QWebPage *cur_page_ = nullptr;
};

