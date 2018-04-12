#include "WebView.h"

#include <iostream>

#include <QtCore/QFile>
#include <QtGui/QPaintEvent>
#include <QtWebKitWidgets/QWebFrame>

#include "WebPage.h"

#include "moc_WebView.cpp"

void WebView::paintEvent(QPaintEvent *ev) {
    if (!page())
        return;

    QSize sz = size();
    QWebFrame *frame = page()->mainFrame();

     
    const auto &r = ev->region().boundingRect();

    if (r.x() + r.width() > sz.width()) return;
    if (r.y() + r.height() > sz.height()) return;

    if (delayed_regions_.size() < 64) {
        delayed_regions_.push_back(ev->region());
    }
}

void WebView::ProcessDelayedRegions() {
    const int BYTES_PER_PIXEL = 4;

    if (p_framebuf_) {
        QSize sz = size();
        auto *p_buf = (uchar *)p_framebuf_;
        QWebFrame *frame = page()->mainFrame();
        QImage img(p_buf, sz.width(), sz.height(), BYTES_PER_PIXEL * sz.width(), QImage::Format_ARGB32);
        QPainter p(&img);
        p.setRenderHints(renderHints());

        QPoint scroll_pos = frame->scrollPosition();
        if (scroll_pos != prev_scroll_pos_) {
            prev_scroll_pos_ = scroll_pos;

            frame->render(&p, { 0, 0, sz.width(), sz.height() });

            updated_regions_.clear();
            updated_regions_.emplace_back(0, 0, sz.width(), sz.height());
        } else {
            for (const auto &r : delayed_regions_) {
                frame->render(&p, r);
                updated_regions_.push_back(r);
            }
        }
        delayed_regions_.clear();
    }
}

QWebPage* WebView::page() const {
    if (!cur_page_) {
        auto *that = const_cast<WebView *>(this);
        cur_page_ = new WebPage(that);
        that->setPage(cur_page_);
    }
    return cur_page_;
}

void WebView::load(const QUrl &url) {
    page()->mainFrame()->load(url);
}
