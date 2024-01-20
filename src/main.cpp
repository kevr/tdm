#include "app.h"
#include "lib/curses.h"
#include "util/argparse.h"
#include "util/termio.h"
#include <format>
#include <iostream>

#define PROJECT_NAME "tdm"
#define PROJECT_VER "0.1"

int main(int argc, char **argv)
{
    static struct option opts[] = {
        // Long-only option (--version):
        {"version", no_argument, nullptr, OPT_LONG},

        // Long & short option (--verbose, -v):
        // {"verbose", no_argument, nullptr, 'v'},

        // End of array
        {nullptr, 0, nullptr, 0},
    };

    auto args = tdm::Args(opts);
    if (int rc = args.parse(argc, argv)) {
        return rc;
    }

    if (args.has("version")) {
        return tdm::print("{}\n", PROJECT_VER);
    }

    auto app = App();
    return app.run();
}
