#include "app.h"
#include "lib/curses.h"
#include "util/argparse.h"
#include "util/termio.h"

#define PROJECT_NAME "tdm"
#define PROJECT_VER "0.1"

int main(int argc, const char *argv[])
{
    static struct option opts[] = {
        // Long-only option (--option):
        {"version", no_argument, nullptr, OPT_LONG},

        // Long & short option (--option, -o):
        {"help", no_argument, nullptr, 'h'},

        // End of array
        {nullptr, 0, nullptr, 0},
    };

    auto args = tdm::Args(opts)
                    .describe("help", 'h', "Print this help summary")
                    .describe("version", "Print the program version");
    if (int rc = args.parse(argc, argv)) {
        return rc;
    }

    if (args.has("help")) {
        return tdm::print("{}\n", args.help());
    } else if (args.has("version")) {
        return tdm::print("{}\n", PROJECT_VER);
    }

    auto app = App();
    return app.run();
}
