#include "ClientApp.h"

#undef main
int main(int argc, char *argv[]) {
    return ClientApp().Run(std::vector<std::string>(argv + 1, argv + argc));
}
