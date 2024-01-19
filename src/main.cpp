#include "app.h"
#include "lib/curses.h"
#include "util/termio.h"
#include <format>
#include <iostream>

#define PROJECT_NAME "tdm"
#define PROJECT_VER "0.1"

int main(int argc, char **argv)
{
    if (argc != 1) {
        return tdm::error(1, "{} takes no arguments.\n", argv[0]);
    }

    tdm::print("{} {}\n", PROJECT_NAME, PROJECT_VER);
    auto app = App();
    return app.run();
}
