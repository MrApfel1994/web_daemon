#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <cmath>
#include <cstdio>
#include <cstdlib>

static void handle_assert(bool passed, const char* assert, const char* file, long line) {
    if (!passed) {
        printf("Assertion failed %s in %s at line %ld\n", assert, file, line);
        exit(-1);
    }
}

#ifndef __EMSCRIPTEN__
#define require(x) handle_assert(x, #x , __FILE__, __LINE__ )

#define require_throws(...) {           \
            bool _ = false;             \
            try {                       \
                __VA_ARGS__             \
            } catch (...) {             \
                _ = true;               \
            }                           \
            require(_);                 \
        }

#define require_nothrow(...) {          \
            bool _ = false;             \
            try {                       \
                __VA_ARGS__             \
            } catch (...) {             \
                _ = true;               \
            }                           \
            require(!_);                \
        }
#else
#define require_throws(expr) {}
#define require_nothrow(expr) {}
#endif

class Approx {
public:
    Approx(double val, double eps = 0.001) : val(val), eps(eps) {
        require(eps > 0);
    }

    double val, eps;
};

inline bool operator==(double val, const Approx &app) {
    return std::abs(val - app.val) < app.eps;
}

inline bool operator==(float val, const Approx &app) {
    return std::abs(val - app.val) < app.eps;
}

#endif