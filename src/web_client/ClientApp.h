#pragma once

#include <memory>
#include <string>
#include <vector>

#include <common/MsgDef.h>
#include <common/PipeClient.h>
#include <common/Process.h>
#include <common/SharedMemory.h>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

class ClientApp {
    SDL_Renderer    *renderer_ = nullptr;
    SDL_Texture     *texture_ = nullptr;
    SDL_Window		*window_ = nullptr;

	WD::Process daemon_proc_;
    WD::PipeClient pipe_;
    WD::SharedMemory framebuf_;

    bool quit_;
    int width_, height_;
    int remote_width_, remote_height_;
    bool frame_request_needed_, frame_updated_;

    int updated_regions_count_;
    WD::Region updated_regions_[16];

	void Receive();
    void PollEvents();
public:
    ClientApp();
    ~ClientApp() = default;

    int Init(int w, int h);
    void Destroy();

    void Frame();

    int Run(const std::vector<std::string> &args);

    bool terminated() const {
        return quit_;
    }
};
