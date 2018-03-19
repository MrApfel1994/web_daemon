#include "WebApp.h"

#include <algorithm>
#include <iostream>

#include <QtGui/QApplication>
#include <QtGui/QWheelEvent>

#include <common/MsgDef.h>
#include <common/PipeServer.h>
#include <common/Process.h>

#include "WebPage.h"
#include "WebView.h"

#include "moc_WebApp.cpp"

namespace WebAppInternal {
#ifdef _WIN32
const char *FRAMEBUF_BASE_NAME = "Local\\FrameBuf_";
#else
const char *FRAMEBUF_BASE_NAME = "FrameBuf_";
#endif
}

WebApp::WebApp(const char *app_id, WebView *web_view, uint32_t parent_id, std::ostream &log_stream)
    : running_(false), app_id_(app_id), w_(0), h_(0), web_view_(web_view), parent_id_(parent_id), log_stream_(log_stream) {
    connect(web_view_, SIGNAL(loadStarted()), this, SLOT(OnLoadStarted()));
    connect(web_view_, SIGNAL(loadProgress(int)), this, SLOT(OnLoadProgress(int)));
    connect(web_view_, SIGNAL(loadFinished(bool)), this, SLOT(OnLoadFinished(bool)));
    connect(web_view_, SIGNAL(titleChanged(const QString&)), this, SLOT(OnTitleChanged(const QString&)));
    connect(web_view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(OnUrlChanged(const QUrl&)));

    auto *page = (WebPage *)web_view_->page();
    connect(page, SIGNAL(JsConsoleMessage(const QString &, int, const QString &)), this, SLOT(OnJsConsole(const QString &, int, const QString &)));

    qRegisterMetaType<QEvent *>("QEvent *");

    if (parent_id_) {
        log_stream_ << "Parent id is " << parent_id_ << std::endl;
    }
}

WebApp::~WebApp() {
}

int WebApp::Run() {
    try {
        log_stream_ << "Opening pipe " << app_id_ << std::endl;
        auto pipe = WD::PipeServer{ app_id_.c_str() };

        if (!pipe.Connect()) {
            log_stream_ << "Failed to make connection on named pipe." << std::endl;
            return -1;
        }

        pipe.WaitForEvent(30000);

        log_stream_ << "Pipe connected." << std::endl;

        {
            // Notify we are ready
            WD::AppStartedMsg msg = {};
            msg.msg_type = WD::AppStarted;
            msg.protocol = WD::ProtocolVersion;

            log_stream_ << "Writing pipe." << std::endl;

            if (!pipe.Write(&msg, sizeof(msg))) {
                log_stream_ << "Write pipe failed. " << std::endl;

                return -1;
            }

            pipe.WaitForEvent(10000);
        }

        uint32_t written = pipe.bytes_transfered();
        log_stream_ << "written = " << written << std::endl;

        char in_buf[WD::MaxMsgLen], out_buf[WD::MaxMsgLen];

        if (!pipe.Read(in_buf, sizeof(in_buf))) {
            log_stream_ << "Read failed!" << std::endl;
        }

        running_ = true;
        while (running_) {
            //log_stream_ << "Waiting for message state " << pipe.state() << std::endl;
            if (!pipe.WaitForEvent(1000)) {
                //log_stream_ << "Wait failed! state " << pipe.state() << std::endl;
            }

            if (pipe.IsAsyncIOComplete()) {
                if (pipe.state() == WD::PipeServer::Reading) {
                    uint32_t bytes_read = pipe.bytes_transfered();
                    pipe.reset();
                    if (bytes_read) {
                        uint32_t out_size = 0;
                        ProcessMessage(in_buf, bytes_read, out_buf, out_size);

                        if (out_size) {
                            if (!pipe.Write(out_buf, out_size)) {
                                log_stream_ << "Write failed!" << std::endl;
                            }
                        } else {
                            if (!pipe.Read(in_buf, sizeof(in_buf))) {
                                log_stream_ << "Read failed!" << std::endl;
                            }
                        }
                    }
                } else if (pipe.state() == WD::PipeServer::Writing) {
                    pipe.reset();

                    std::lock_guard<std::mutex> _(mtx_);
                    if (!delayed_messages_.empty()) {
                        const auto &msg = delayed_messages_.front();
                        if (!pipe.Write(&msg[0], (uint32_t)msg.size())) {
                            log_stream_ << "Write failed!" << std::endl;
                        }
                        delayed_messages_.erase(delayed_messages_.begin());
                    } else {
                        if (!pipe.Read(in_buf, sizeof(in_buf))) {
                            log_stream_ << "Read failed!" << std::endl;
                        }
                    }
                }
            } else {
                log_stream_ << "!!!!!!!!!!" << std::endl;
            }

            if (parent_id_) {
                if (!WD::Process::IsStillRunning(parent_id_)) {
                    log_stream_ << "Parent process with id " << parent_id_ << " is dead, exiting..." << std::endl;
                    running_ = false;
                }
            }
        }
        log_stream_ << "Exiting loop!" << std::endl;
    } catch (...) {
        log_stream_ << "Failed to create pipe." << std::endl;
        return -1;
    }

    return 0;
}

void WebApp::ProcessMessage(const void *in_buf, uint32_t in_size, void *out_buf, uint32_t &out_size) {
    using namespace WebAppInternal;

    if (!in_size) return;

    uint8_t msg_type;
    memcpy(&msg_type, in_buf, sizeof(msg_type));

    //log_stream_ << "msg_type = " << (int)msg_type << std::endl;
    switch (msg_type) {
    case WD::ResizeView: {
        if (in_size == sizeof(WD::ResizeViewMsg)) {
            const auto *req = (const WD::ResizeViewMsg *)in_buf;
            log_stream_ << req->width << "x" << req->height << std::endl;

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
            log_stream_ << "Resize message has wrong size! " << in_size << std::endl;
        }
        break;
        case WD::FrameRequest: {
            if (in_size == sizeof(WD::FrameRequestMsg)) {
                const auto *req = (WD::FrameRequestMsg *)in_buf;

                QMetaObject::invokeMethod(web_view_, "ProcessDelayedRegions", Qt::BlockingQueuedConnection);
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
                    log_stream_ << "MouseDown " << msg->x << " " << msg->y << std::endl;
                    ev = new QMouseEvent{ QEvent::MouseButtonPress, { msg->x, msg->y }, Qt::LeftButton, {}, {} };
                } else if (msg->ev_type == WD::MouseUp) {
                    log_stream_ << "MouseUp " << msg->x << " " << msg->y << std::endl;
                    ev = new QMouseEvent{ QEvent::MouseButtonRelease, { msg->x, msg->y }, Qt::LeftButton, {}, {} };
                } else if (msg->ev_type == WD::MouseWheel) {
                    ev = new QWheelEvent{ { msg->x, msg->y }, msg->dy, {}, {} };
                } else if (msg->ev_type == WD::KeyDown) {
                    log_stream_ << "KeyDown " << msg->keycode << std::endl;
                    auto text = (msg->modifiers & Qt::ShiftModifier) ? "" : QKeySequence(msg->keycode).toString().toLower();
                    if (text.length() > 1) text = "";
                    ev = new QKeyEvent{ QEvent::KeyPress, msg->keycode, (Qt::KeyboardModifier)msg->modifiers, text };
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
                    log_stream_ << "Url change requested: " << msg->url << std::endl;
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
            log_stream_ << "Shutdown received." << std::endl;
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

    w_ = w;
    h_ = h;

    QMetaObject::invokeMethod(web_view_, "Resize", Qt::BlockingQueuedConnection,
                              Q_ARG(int, 0), Q_ARG(int, 0), Q_ARG(void *, nullptr));
    framebuf_mem_ = {};

    try {
        std::string fbuf_name = FRAMEBUF_BASE_NAME + app_id_;
        framebuf_mem_ = { fbuf_name.c_str(), (size_t)w * h * 4, WD::Create | WD::ReadWrite };
    } catch (...) {
        log_stream_ << "FrameBuffer allocation failed: " << w << "x" << h << std::endl;
        return false;
    }

    log_stream_ << "Resizing start " << w << "x" << h << std::endl;
    QMetaObject::invokeMethod(web_view_, "Resize", Qt::BlockingQueuedConnection,
                              Q_ARG(int, w), Q_ARG(int, h), Q_ARG(void *, framebuf_mem_.buf()));
    log_stream_ << "Resizing end " << w << "x" << h << std::endl;

    return true;
}

void WebApp::OnLoadStarted() {
    std::vector<uint8_t> msg(sizeof(WD::PageEventLiteMsg));

    auto *p_msg = (WD::PageEventLiteMsg *)msg.data();

    p_msg->msg_type = WD::PageEvent;
    p_msg->ev_type = WD::LoadStarted;

    std::lock_guard<std::mutex> _(mtx_);
    delayed_messages_.emplace_back(std::move(msg));

    log_stream_ << "OnLoadStarted" << std::endl;
}

void WebApp::OnLoadProgress(int progress) {
    std::vector<uint8_t> msg(sizeof(WD::PageEventLiteMsg));

    auto *p_msg = (WD::PageEventLiteMsg *)msg.data();

    p_msg->msg_type = WD::PageEvent;
    p_msg->ev_type = WD::LoadProgress;
    p_msg->progress = (uint8_t)progress;

    std::lock_guard<std::mutex> _(mtx_);
    delayed_messages_.emplace_back(std::move(msg));

    log_stream_ << "OnLoadProgress " << progress << std::endl;
}

void WebApp::OnLoadFinished(bool result) {
    std::vector<uint8_t> msg(sizeof(WD::PageEventLiteMsg));

    auto *p_msg = (WD::PageEventLiteMsg *)msg.data();

    p_msg->msg_type = WD::PageEvent;
    p_msg->ev_type = WD::LoadFinished;

    std::lock_guard<std::mutex> _(mtx_);
    delayed_messages_.emplace_back(std::move(msg));

    log_stream_ << "OnLoadFinished" << std::endl;
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

    log_stream_ << "OnTitleChanged " << str_title << std::endl;
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

    log_stream_ << "OnUrlChanged " << str_url << std::endl;
}

void WebApp::OnJsConsole(const QString &id, int line, const QString &msg_str) {
    std::vector<uint8_t> msg;
    msg.resize(sizeof(WD::PageEventMsg), 0);

    auto *p_msg = (WD::PageEventMsg *)msg.data();

    p_msg->msg_type = WD::PageEvent;
    p_msg->ev_type = WD::JsMessage;

    if (!id.isEmpty()) {
        const char *_id = id.toLocal8Bit().constData();
        memcpy(&p_msg->js.id[0], _id, std::min<size_t>(sizeof(p_msg->js.id), strlen(_id) + 1));
        
        memcpy(&p_msg->js.line[0], &line, sizeof(line));

        const char *_msg_str = msg_str.toLocal8Bit().constData();
        memcpy(&p_msg->js.msg[0], _msg_str, std::min<size_t>(sizeof(p_msg->js.msg), strlen(_msg_str) + 1));

        log_stream_ << "[JS]: " << id.toLocal8Bit().constData() << "(" << line << "): " << msg_str.toLocal8Bit().constData() << std::endl;
    } else {
        const char *_msg_str = msg_str.toLocal8Bit().constData();
        memcpy(&p_msg->js.msg[0], _msg_str, std::min<size_t>(sizeof(p_msg->js.msg) - 1, strlen(_msg_str) + 1));

        log_stream_ << "[JS]: " << msg_str.toLocal8Bit().constData() << std::endl;
    }

    std::lock_guard<std::mutex> _(mtx_);
    delayed_messages_.emplace_back(std::move(msg));
}