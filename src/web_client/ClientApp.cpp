#include "ClientApp.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

ClientApp::ClientApp() : quit_(false) {
    remote_width_ = 0;
    remote_height_ = 0;
    frame_request_needed_ = true;
    frame_updated_ = false;
}

int ClientApp::Init(int w, int h, const std::string &url) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    width_ = w;
    height_ = h;

    window_ = SDL_CreateWindow("View", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        const char *s = SDL_GetError();
        printf("%s\n", s);
    }
    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!texture_) {
        const char *s = SDL_GetError();
        printf("%s\n", s);
    }

    SDL_GL_SetSwapInterval(1);

    try {
        daemon_proc_ = WD::Process{ (std::string("web_daemon test_id ") + url).c_str(), false };
        WD::Process::Sleep(1000);
        pipe_ = WD::PipeClient{ "test_id" };
    } catch (std::runtime_error &) {
        return -1;
    }

    uint8_t buf[WD::MaxMsgLen];
    uint32_t bytes_read = 0;
    if (pipe_.Read(&buf[0], sizeof(buf), &bytes_read) && 
        buf[0] == WD::AppStarted && bytes_read == sizeof(WD::AppStartedMsg)) {
        const auto *msg = (const WD::AppStartedMsg *)buf;

        if (msg->protocol != WD::ProtocolVersion) return -1;

        WD::ResizeViewMsg req = {};
        req.msg_type = WD::ResizeView;
        req.width = (uint16_t)w;
        req.height = (uint16_t)h;

        if (!pipe_.Write(&req, sizeof(req))) {
            return false;
        }

        bytes_read = 0;
        if (pipe_.Read(buf, sizeof(buf), &bytes_read) &&
            buf[0] == WD::ViewResized && bytes_read == sizeof(WD::ViewResizedMsg)) {
            const auto *resp = (const WD::ViewResizedMsg *)buf;

            if (resp->result != WD::Success || resp->width != w || resp->height != h) return -1;

            remote_width_ = resp->width;
            remote_height_ = resp->height;

            try {
                size_t size = (size_t)remote_width_ * remote_height_ * 4;
                framebuf_ = { resp->framebuf_name, size, WD::ReadOnly };
            } catch (...) {
                return -1;
            }
            
            pipe_.SetBlocking(false);

            //
            /*WD::PageEventMsg req = {};
            req.msg_type = WD::PageEvent;
            req.ev_type = WD::UrlRequest;
            strcpy(req.url, "https://google.com");

            pipe_.Write(&req, sizeof(req));*/
        }
    }

    frame_request_needed_ = true;

    return 0;
}

void ClientApp::Destroy() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void ClientApp::Frame() {
    if (frame_request_needed_) {
        WD::FrameRequestMsg req = { 0 };
        req.msg_type = WD::FrameRequest;
        req.flags = WD::LockFullHorizontalSpan;

        /*if (pipe_.Write(&req, sizeof(req))) {
            frame_request_needed_ = false;
        }*/
    }
}

int ClientApp::Run(const std::vector<std::string> &args) {
    const int w = 1280; const int h = 720;

	std::string url = "http://html5test.com";
	if (!args.empty()) {
		url = args[0];
	}

    if (Init(w, h, url) < 0) {
        return -1;
    }

    while (!terminated()) {
        this->Receive();

        this->PollEvents();

        this->Frame();

        frame_updated_ = framebuf_.buf() != nullptr;
        if (frame_updated_ && width_ == remote_width_ && height_ == remote_height_) {
            SDL_UpdateTexture(texture_, nullptr, framebuf_.buf(), width_ * sizeof(Uint32));

            /*for (int i = 0; i < updated_regions_count_; i++) {
                SDL_Rect rect;
                rect.x = updated_regions_[i].x;
                rect.y = updated_regions_[i].y;
                rect.w = updated_regions_[i].w;
                rect.h = updated_regions_[i].h;

                const uint8_t *pixels = (const uint8_t *)framebuf_.buf();
                SDL_UpdateTexture(texture_, &rect, &pixels[(rect.y * width_ + rect.x) * 4], width_ * 4);
            }*/

            frame_updated_ = false;
            frame_request_needed_ = true;
        }

        //SDL_RenderClear(renderer_);
        SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
        SDL_RenderPresent(renderer_);
    }

    this->Destroy();

    return 0;
}

void ClientApp::Receive() {
    uint8_t in_buf[WD::MaxMsgLen];
    uint32_t bytes_read = 0;
    if (pipe_.Read(&in_buf, sizeof(in_buf), &bytes_read)) {
        switch (in_buf[0]) {
        case WD::ViewResized:
            if (bytes_read == sizeof(WD::ViewResizedMsg)) {
                const auto *msg = (const WD::ViewResizedMsg *)in_buf;
                
                remote_width_ = msg->width;
                remote_height_ = msg->height;

                size_t size = (size_t)remote_width_ * remote_height_ * 4;
                framebuf_ = { msg->framebuf_name, size, WD::ReadOnly };
            }
            break;
        case WD::FrameUpdated:
            if (bytes_read == sizeof(WD::FrameUpdatedMsg)) {
                const auto *msg = (const WD::FrameUpdatedMsg *)in_buf;

                frame_updated_ = frame_updated_ || (msg->updated_regions_count != 0);
                updated_regions_count_ = msg->updated_regions_count;
                memcpy(&updated_regions_[0], msg->updated_regions, sizeof(msg->updated_regions));

                if (!frame_updated_) {
                    frame_request_needed_ = true;
                }
            }
            break;
        default:
            break;
        }
    }
}

void ClientApp::PollEvents() {
    SDL_Event e = { 0 };
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_KEYDOWN: {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                quit_ = true;
                return;
            }
        }
        break;
        case SDL_KEYUP:
            
            break;
        case SDL_MOUSEBUTTONDOWN: {
            WD::InputEventMsg msg = { 0 };
            msg.msg_type = WD::InputEvent;
            msg.ev_type = WD::MouseDown;
            msg.x = (int32_t)e.motion.x;
            msg.y = (int32_t)e.motion.y;

            pipe_.Write(&msg, sizeof(msg));
        }
        break;
        case SDL_MOUSEBUTTONUP: {
            WD::InputEventMsg msg = { 0 };
            msg.msg_type = WD::InputEvent;
            msg.ev_type = WD::MouseUp;
            msg.x = (int32_t)e.motion.x;
            msg.y = (int32_t)e.motion.y;

            pipe_.Write(&msg, sizeof(msg));
        }
        break;
        case SDL_QUIT: {
            quit_ = true;
            return;
        }
        case SDL_MOUSEMOTION: {
            WD::InputEventMsg msg = { 0 };
            msg.msg_type = WD::InputEvent;
            msg.ev_type = WD::MouseMove;
            msg.x = (int32_t)e.motion.x;
            msg.y = (int32_t)e.motion.y;

            if (!pipe_.Write(&msg, sizeof(msg))) {
                printf("-Write failed!\n");
            }
        }
        break;
        case SDL_MOUSEWHEEL: {
            WD::InputEventMsg msg = { 0 };
            msg.msg_type = WD::InputEvent;
            msg.ev_type = WD::MouseWheel;
            msg.y = (int32_t)e.wheel.y * 100;

            pipe_.Write(&msg, sizeof(msg));
        }
        break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                SDL_RenderPresent(renderer_);

                SDL_DestroyTexture(texture_);
                texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                             e.window.data1, e.window.data2);

                remote_width_ = 0;
                remote_height_ = 0;
                framebuf_ = {};

                width_ = (int)e.window.data1;
                height_ = (int)e.window.data2;

                WD::ResizeViewMsg msg = { 0 };
                msg.msg_type = WD::ResizeView;
                msg.width = (uint16_t)width_;
                msg.height = (uint16_t)height_;

                pipe_.Write(&msg, sizeof(msg));
            }
            break;
        default:
            return;
        }
    }
}
