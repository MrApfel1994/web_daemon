#include "test_common.h"

#include <cstring>
#include <thread>

#include "../PipeClient.h"
#include "../PipeServer.h"
#include "../Process.h"

namespace {
const char msg1[] = "test_message";
const char msg2[] = "test_answer";
}

void test_pipe_child();

void test_pipe() {
    require_nothrow(
        auto pr = WD::Process{ "test_common --child 2" };
        WD::PipeClient pipe;

        int time = 0;
        while (time < 10000) {
        try {
            pipe = WD::PipeClient{ "TestPipe2134567891" };
            break;
        } catch (...) { }

            WD::Process::Sleep(100);
            time += 100;
        }

        pipe.SetBlocking(false);

        time = 0;
        while (time < 10000) {
        char in_buf[128];
            uint32_t bytes_read = 0;
            if (pipe.Read(&in_buf[0], sizeof(in_buf), &bytes_read)) {
                require(bytes_read == sizeof(msg1));
                require(strcmp(in_buf, msg1) == 0);

                require(pipe.Write(&msg2[0], sizeof(msg2)));
                break;
            }

            WD::Process::Sleep(100);
            time += 100;
        }

        require(time < 10000);

        pr.WaitForCompletion();
    );
}

void test_pipe_child() {
    require_nothrow(
        auto pipe = WD::PipeServer{ "TestPipe2134567891" };
        require(pipe.Connect());
        pipe.WaitForEvent(10000);
        require(pipe.Write(msg1, sizeof(msg1)));
        pipe.WaitForEvent(10000);
        require(pipe.IsAsyncIOComplete());
        require(pipe.state() == WD::PipeServer::Writing);

        char in_buf[128];
        require(pipe.Read(&in_buf[0], sizeof(in_buf)));
        pipe.WaitForEvent(10000);
        require(pipe.IsAsyncIOComplete());
        require(pipe.state() == WD::PipeServer::Reading);
        require(pipe.bytes_transfered() == sizeof(msg2));
        require(strcmp(in_buf, msg2) == 0);
    );
}
