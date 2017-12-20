#include "WebView.h"

#include <iostream>

#include <QtCore/QFile>
#include <QtGui/QPaintEvent>
#include <QtWebKit/QWebFrame>

#include "moc_WebView.cpp"

void WebView::paintEvent(QPaintEvent *ev) {
	if (!page())
		return;

	std::lock_guard<std::mutex> _(mtx_);

    {
        //std::unique_lock<std::mutex> lk(mtx_);
        //cv_.wait(lk, [this] { return frame_requested_; });
    }

    const int BYTES_PER_PIXEL = 4;

	if (p_framebuf_) {
		QSize sz = size();
		auto *p_buf = (uchar *)p_framebuf_;
		QWebFrame *frame = page()->mainFrame();

		QPoint scroll_pos = frame->scrollPosition();
		if (scroll_pos != prev_scroll_pos_) {
			auto scroll_delta = scroll_pos - prev_scroll_pos_;

			int start_y = 0 + scroll_delta.y();
			if (start_y < 0) start_y = 0;
			if (start_y > sz.height() - 1) start_y = sz.height() - 1;

			int end_y = 0 - scroll_delta.y();
			if (end_y < 0) end_y = 0;
			if (end_y > sz.height() - 1) end_y = sz.height() - 1;

			memmove(p_buf + end_y * sz.width() * BYTES_PER_PIXEL,
					p_buf + start_y * sz.width() * BYTES_PER_PIXEL,
					(size_t)(sz.height() - std::abs(end_y - start_y)) * sz.width() * BYTES_PER_PIXEL);

			prev_scroll_pos_ = scroll_pos;

			//std::lock_guard<std::mutex> _(mtx_);
			updated_regions_.clear();
			updated_regions_.emplace_back(0, 0, (int)sz.width(), (int)sz.height());
		} else {
			//std::lock_guard<std::mutex> _(mtx_);
			updated_regions_.push_back(ev->region());
            frame_requested_ = false;
		}

		QImage img(p_buf, sz.width(), sz.height(), BYTES_PER_PIXEL * sz.width(), QImage::Format_ARGB32);

        //std::lock_guard<std::mutex> _(mtx_);

        const auto &r = ev->region().boundingRect();

        if (r.x() + r.width() > sz.width()) return;
        if (r.y() + r.height() > sz.height()) return;

		QPainter p(&img);
		p.setRenderHints(renderHints());
        frame->render(&p, ev->region());

		{
			/*QPainter p(this);
			p.setRenderHints(renderHints());
			frame->render(&p, ev->region());*/
		}
	}
}