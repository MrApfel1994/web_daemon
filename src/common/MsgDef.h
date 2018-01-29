#pragma once

#include <cstdint>

namespace WD {
    enum eMsgType : uint8_t {
        AppStarted,
        ResizeView,
        ViewResized,
        FrameRequest,
        FrameUpdated,
        InputEvent,
        PageEvent,
        Shutdown,
    };

    enum eEvType : uint8_t {
        MouseMove,
        MouseWheel,
        MouseDown,
        MouseUp,
        KeyDown,
        KeyUp,
    };

    enum ePgEvType : uint8_t {
        UrlRequest,
        UrlResponse,
        PageForward,
        PageBackward,
        PageReload,
        LoadStarted,
        LoadProgress,
        LoadFinished,
        TitleChanged,
    };

    enum eFrameReqFlags : uint8_t {
        LockFullHorizontalSpan = 1,
    };

    enum eResult { Success, Fail };

    const uint32_t ProtocolVersion = 1;
    const uint32_t MaxMsgLen = 256;

    struct Region { uint16_t x, y, w, h; };
    static_assert(sizeof(Region) == 8, "!");

    struct EmptyMsg {
        uint8_t msg_type;
    };
    static_assert(sizeof(EmptyMsg) == 1, "!");

    struct AppStartedMsg {
        uint8_t msg_type;
        uint32_t protocol;
    };
    static_assert(sizeof(AppStartedMsg) == 8, "!");

    struct ResizeViewMsg {
        uint8_t msg_type;
        uint16_t width;
        uint16_t height;
    };
    static_assert(sizeof(ResizeViewMsg) == 6, "!");

    struct ViewResizedMsg {
        uint8_t msg_type;
        uint8_t result;
        uint16_t width;
        uint16_t height;
        char framebuf_name[64];
    };
    static_assert(sizeof(ViewResizedMsg) == 70, "!");

    struct FrameRequestMsg {
        uint8_t msg_type;
        uint8_t flags;
    };
    static_assert(sizeof(FrameRequestMsg) == 2, "!");

    const int MaxUpdatedRegions = 8;
    struct FrameUpdatedMsg {
        uint8_t msg_type;
        uint8_t updated_regions_count;
        Region updated_regions[MaxUpdatedRegions];
    };
    static_assert(sizeof(FrameUpdatedMsg) == 66, "!");

    struct InputEventMsg {
        uint8_t msg_type;
        uint8_t ev_type;
        int32_t x, y;
        int32_t keycode;
    };
    static_assert(sizeof(InputEventMsg) == 16, "!");

    struct PageEventMsg {
        uint8_t msg_type;
        uint8_t ev_type;
        union {
            char url[254];
            char title[254];
        };
    };
    static_assert(sizeof(PageEventMsg) == 256, "!");

    struct PageEventLiteMsg {
        uint8_t msg_type;
        uint8_t ev_type;
        uint8_t progress;
    };
    static_assert(sizeof(PageEventLiteMsg) == 3, "!");
}