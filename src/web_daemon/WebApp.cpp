#include "WebApp.h"

#include <iostream>

#include <QtGui/QApplication>
#include <QtGui/QWheelEvent>

#include <common/MsgDef.h>
#include <common/PipeServer.h>

#include "WebView.h"

#include "moc_WebApp.cpp"

namespace WebAppInternal {
#ifdef _WIN32
	const char *FRAMEBUF_BASE_NAME = "Local\\FrameBuf_";
#else
	const char *FRAMEBUF_BASE_NAME = "FrameBuf_";
#endif
}

WebApp::WebApp(const char *app_id, WebView *web_view)
	: running_(false), app_id_(app_id), w_(0), h_(0), web_view_(web_view) {
	connect(web_view_, SIGNAL(loadStarted()), this, SLOT(OnLoadStarted()));
	connect(web_view_, SIGNAL(loadProgress(int)), this, SLOT(OnLoadProgress(int)));
	connect(web_view_, SIGNAL(loadFinished(bool)), this, SLOT(OnLoadFinished(bool)));
	connect(web_view_, SIGNAL(titleChanged(const QString&)), this, SLOT(OnTitleChanged(const QString&)));
	connect(web_view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(OnUrlChanged(const QUrl&)));

    qRegisterMetaType<QEvent *>("QEvent *");
}

WebApp::~WebApp() {
}

int WebApp::Run() {
	try {
		std::cout << "Openging pipe " << app_id_ << std::endl;
		auto pipe = WD::PipeServer{ app_id_.c_str() };

		if (!pipe.Connect()) {
			std::cout << "Failed to make connection on named pipe." << std::endl;
			return -1;
		}

		pipe.WaitForEvent(30000);

		std::cout << "Pipe connected." << std::endl;

		{	// Notify we are ready
			WD::AppStartedMsg msg = {};
			msg.msg_type = WD::AppStarted;
			msg.protocol = WD::ProtocolVersion;

			std::cout << "Writing pipe." << std::endl;

			if (!pipe.Write(&msg, sizeof(msg))) {
				std::cout << "Write pipe failed. " << std::endl;

				return -1;
			}

			pipe.WaitForEvent(10000);
		}

		uint32_t written = pipe.bytes_transfered();
		std::cout << "written = " << written << std::endl;

		char in_buf[WD::MaxMsgLen], out_buf[WD::MaxMsgLen];

		if (!pipe.Read(in_buf, sizeof(in_buf))) { std::cout << "Read failed!" << std::endl; }

		running_ = true;
		while (running_) {
			//std::cout << "Waiting for message state " << pipe.state() << std::endl;
			if (!pipe.WaitForEvent(1000)) {
				//std::cout << "Wait failed! state " << pipe.state() << std::endl;
			}

			if (pipe.IsAsyncIOComplete()) {
                //std::cout << "+++" << std::endl;
				if (pipe.state() == WD::PipeServer::Reading) {
					uint32_t bytes_read = pipe.bytes_transfered();
					pipe.reset();
                    //std::cout << bytes_read << std::endl;
					if (bytes_read) {
						uint32_t out_size = 0;
						ProcessMessage(in_buf, bytes_read, out_buf, out_size);

						if (out_size) {
							if (!pipe.Write(out_buf, out_size)) {
								std::cout << "Write failed!" << std::endl;
							}
						} else {
							if (!pipe.Read(in_buf, sizeof(in_buf))) { std::cout << "Read failed!" << std::endl; }
						}
					}
				} else if (pipe.state() == WD::PipeServer::Writing) {
					pipe.reset();

					std::lock_guard<std::mutex> _(mtx_);
					if (!delayed_messages_.empty()) {
						const auto &msg = delayed_messages_.front();
						if (!pipe.Write(&msg[0], (uint32_t)msg.size())) { std::cout << "Write failed!" << std::endl; }
						delayed_messages_.erase(delayed_messages_.begin());
					} else {
						if (!pipe.Read(in_buf, sizeof(in_buf))) { std::cout << "Read failed!" << std::endl; }
					}
				}
			} else {
				//std::cout << "!!!!!!!!!!" << std::endl;
			}
		}
		std::cout << "Exiting loop!" << std::endl;
	} catch (...) {
		std::cout << "Failed to create pipe." << std::endl;
		return -1;
	}

	return 0;
}

void WebApp::ProcessMessage(const void *in_buf, uint32_t in_size, void *out_buf, uint32_t &out_size) {
	using namespace WebAppInternal;

	if (!in_size) return;

	uint8_t msg_type;
	memcpy(&msg_type, in_buf, sizeof(msg_type));

    //std::cout << "msg_type = " << (int)msg_type << std::endl;
	switch (msg_type) {
		case WD::ResizeView: {
			if (in_size == sizeof(WD::ResizeViewMsg)) {
				const auto *req = (const WD::ResizeViewMsg *)in_buf;
				std::cout << req->width << "x" << req->height << std::endl;

				bool result = OnResize((int)req->width, (int)req->height);

				std::string fbuf_name = FRAMEBUF_BASE_NAME + app_id_;

				WD::ViewResizedMsg resp = {};
				resp.msg_type = WD::ViewResized;
				resp.result = result ? WD::Success : WD::Fail;
				resp.width = req->width;
				resp.height = req->height;
				strcpy(resp.framebuf_name, fbuf_name.c_str());

				memcpy(out_buf, &resp, sizeof(resp));
				out_size = sizeof(resp);
			} else {
				std::cout << "Resize message has wrong size! " << in_size << std::endl;
			}
			break;
		case WD::FrameRequest: {
			if (in_size == sizeof(WD::FrameRequestMsg)) {
				const auto *req = (WD::FrameRequestMsg *)in_buf;

				std::vector<QRegion> updated_regions = web_view_->StealUpdatedRegions();

				WD::FrameUpdatedMsg resp = {};
				resp.msg_type = WD::FrameUpdated;
				resp.updated_regions_count = 0;

				for (const auto &region : updated_regions) {
                    const auto &rect = region.boundingRect();

                    if (resp.updated_regions_count >= WD::MaxUpdatedRegions) {
                        resp.updated_regions[0].x = 0;
                        resp.updated_regions[0].y = 0;
                        resp.updated_regions[0].w = (uint16_t)w_;
                        resp.updated_regions[0].h = (uint16_t)h_;
                        resp.updated_regions_count = 1;

                        break;
                    }

                    resp.updated_regions[resp.updated_regions_count].x = (uint16_t)rect.x();
                    resp.updated_regions[resp.updated_regions_count].y = (uint16_t)rect.y();
                    resp.updated_regions[resp.updated_regions_count].w = (uint16_t)rect.width();
                    resp.updated_regions[resp.updated_regions_count].h = (uint16_t)rect.height();
                    resp.updated_regions_count++;
				}

				memcpy(out_buf, &resp, sizeof(resp));
				out_size = sizeof(resp);
			}
		}
		break;
        case WD::InputEvent: {
            if (in_size == sizeof(WD::InputEventMsg)) {
                const auto *msg = (const WD::InputEventMsg *)in_buf;

                QEvent *ev = nullptr;

                if (msg->ev_type == WD::MouseMove) {
                    ev = new QMouseEvent{ QEvent::MouseMove, { msg->x, msg->y }, {}, {}, {} };
                } else if (msg->ev_type == WD::MouseDown) {
					std::cout << "MouseDown " << msg->x << " " << msg->y << std::endl;
                    ev = new QMouseEvent{ QEvent::MouseButtonPress, { msg->x, msg->y }, Qt::LeftButton, {}, {} };
                } else if (msg->ev_type == WD::MouseUp) {
					std::cout << "MouseUp " << msg->x << " " << msg->y << std::endl;
                    ev = new QMouseEvent{ QEvent::MouseButtonRelease, { msg->x, msg->y }, Qt::LeftButton, {}, {} };
                } else if (msg->ev_type == WD::MouseWheel) {
                    ev = new QWheelEvent{ { 0, 0 }, msg->y, {}, {} };
                } else if (msg->ev_type == WD::KeyDown) {
					std::cout << "KeyDown " << msg->keycode << std::endl;
					ev = new QKeyEvent{ QEvent::KeyPress, msg->keycode, {}, {} };
				}

                if (ev) {
                    QMetaObject::invokeMethod(web_view_, "ProcessEvent", Qt::QueuedConnection, Q_ARG(QEvent *, ev));
                }
            }
        }
        break;
		case WD::PageEvent: {
			if (in_size == sizeof(WD::PageEventMsg)) {
				const auto *msg = (const WD::PageEventMsg *)in_buf;

				if (msg->ev_type == WD::UrlRequest) {
					std::cout << "Url change requested: " << msg->url << std::endl;
					QMetaObject::invokeMethod(web_view_, "LoadUrl", Qt::QueuedConnection, Q_ARG(QUrl, QUrl{ msg->url }));
				} else if (msg->ev_type == WD::PageBackward) {
					QMetaObject::invokeMethod(web_view_, "PageBackward", Qt::QueuedConnection);
				} else if (msg->ev_type == WD::PageForward) {
					QMetaObject::invokeMethod(web_view_, "PageForward", Qt::QueuedConnection);
				} else if (msg->ev_type == WD::PageReload) {
					QMetaObject::invokeMethod(web_view_, "PageReload", Qt::QueuedConnection);
				}
			}
		}
		break;
		case WD::Shutdown: {
			std::cout << "Shutdown received." << std::endl;
			running_ = false;
		}
        break;
		default:
			break;
		}
	}
}

bool WebApp::OnResize(int w, int h) {
    using namespace WebAppInternal;

	w_ = w; h_ = h;

	QMetaObject::invokeMethod(web_view_, "Resize", Qt::BlockingQueuedConnection,
							  Q_ARG(int, 0), Q_ARG(int, 0), Q_ARG(void *, nullptr));
	framebuf_mem_ = {};

    try {
		std::string fbuf_name = FRAMEBUF_BASE_NAME + app_id_;
        framebuf_mem_ = { fbuf_name.c_str(), (size_t)w * h * 4, WD::Create | WD::ReadWrite };
    } catch (...) {
		std::cout << "FrameBuffer allocation failed: " << w << "x" << h << std::endl;
        return false;
    }
	
	std::cout << "Resizing start " << w << "x" << h << std::endl;
    QMetaObject::invokeMethod(web_view_, "Resize", Qt::BlockingQueuedConnection,
							  Q_ARG(int, w), Q_ARG(int, h), Q_ARG(void *, framebuf_mem_.buf()));
	std::cout << "Resizing end " << w << "x" << h << std::endl;

	return true;
}

void WebApp::OnLoadStarted() {
	std::vector<uint8_t> msg(sizeof(WD::PageEventLiteMsg));

	auto *p_msg = (WD::PageEventLiteMsg *)msg.data();

	p_msg->msg_type = WD::PageEvent;
	p_msg->ev_type = WD::LoadStarted;

	std::lock_guard<std::mutex> _(mtx_);
	delayed_messages_.emplace_back(std::move(msg));

	std::cout << "OnLoadStarted" << std::endl;
}

void WebApp::OnLoadProgress(int progress) {
	std::vector<uint8_t> msg(sizeof(WD::PageEventLiteMsg));

	auto *p_msg = (WD::PageEventLiteMsg *)msg.data();

	p_msg->msg_type = WD::PageEvent;
	p_msg->ev_type = WD::LoadProgress;
	p_msg->progress = (uint8_t)progress;

	std::lock_guard<std::mutex> _(mtx_);
	delayed_messages_.emplace_back(std::move(msg));

	std::cout << "OnLoadProgress " << progress << std::endl;
}

void WebApp::OnLoadFinished(bool result) {
	std::vector<uint8_t> msg(sizeof(WD::PageEventLiteMsg));

	auto *p_msg = (WD::PageEventLiteMsg *)msg.data();

	p_msg->msg_type = WD::PageEvent;
	p_msg->ev_type = WD::LoadFinished;

	std::lock_guard<std::mutex> _(mtx_);
	delayed_messages_.emplace_back(std::move(msg));

	std::cout << "OnLoadFinished" << std::endl;
}

void WebApp::OnTitleChanged(const QString &title) {
	std::vector<uint8_t> msg(sizeof(WD::PageEventMsg));

	auto *p_msg = (WD::PageEventMsg *)msg.data();

	p_msg->msg_type = WD::PageEvent;
	p_msg->ev_type = WD::TitleChanged;

	std::string str_title = title.toLocal8Bit().constData();
	if (str_title.length() >= sizeof(p_msg->title)) str_title.resize(sizeof(p_msg->title) - 1);
	strcpy(p_msg->title, str_title.c_str());

	std::lock_guard<std::mutex> _(mtx_);
	delayed_messages_.emplace_back(std::move(msg));

	std::cout << "OnTitleChanged " << str_title << std::endl;
}

void WebApp::OnUrlChanged(const QUrl &url) {
	std::vector<uint8_t> msg(sizeof(WD::PageEventMsg));

	auto *p_msg = (WD::PageEventMsg *)msg.data();

	p_msg->msg_type = WD::PageEvent;
	p_msg->ev_type = WD::UrlResponse;

	std::string str_url = url.toString().toLocal8Bit().constData();
	if (str_url.length() >= sizeof(p_msg->url)) str_url.resize(sizeof(p_msg->url) - 1);
	strcpy(p_msg->url, str_url.c_str());

	std::lock_guard<std::mutex> _(mtx_);
	delayed_messages_.emplace_back(std::move(msg));

	std::cout << "OnUrlChanged " << str_url << std::endl;
}
