
#include <cstdio>
#include <cstdlib>
#include <cstring>

void test_process();
void test_process_child();
void test_shared_memory();
void test_shared_memory_child();
void test_pipe();
void test_pipe_child();

int main(int argc, char *argv[]) {
    int child_index = -1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--child") == 0 && i + 1 < argc) {
            child_index = atoi(argv[i + 1]);
        }
    }

    if (child_index == -1) {
        test_process();
        test_shared_memory();
        test_pipe();

        puts("OK");
    } else if (child_index == 0) {
        test_process_child();
    } else if (child_index == 1) {
        test_shared_memory_child();
    } else if (child_index == 2) {
        test_pipe_child();
    }

    return 0;
}

