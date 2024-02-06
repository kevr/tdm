#include "app.h"
#include "config.h"
#include "lib/curses.h"
#include "util/argparse.h"
#include "util/logger.h"
#include "util/termio.h"

using namespace tdm;

int main(int argc, const char *argv[])
{
    static struct option opts[] = {
        // Long-only option (--option):
        {"version", no_argument, nullptr, OPT_LONG},

        // Long & short option (--option, -o):
        {"help", no_argument, nullptr, 'h'},
        {"verbose", no_argument, nullptr, 'v'},
        {"log-to", required_argument, nullptr, 'l'},

        // End of array
        {nullptr, 0, nullptr, 0},
    };

    auto args = Args(opts)
                    .describe("version", "Print the program version")
                    .describe("help", 'h', "Print this help summary")
                    .describe("verbose", 'v', "Enable verbose logging")
                    .describe("log-to", 'l', "PATH",
                              "Path to log file [default: stdout]");
    if (int rc = args.parse(argc, argv)) {
        return rc;
    }

    if (args.has("help")) {
        return tdm::print("{}\n", args.help());
    } else if (args.has("version")) {
        return tdm::print("{}\n", PROJECT_VER);
    }

    if (args.has("log-to")) {
        const auto &logfile = args.get("log-to");
        if (!logger.open(logfile)) {
            return tdm::error(1, "unable to open '{}' for writing\n", logfile);
        } else {
            tdm::print("started logging to '{}'\n", logfile);
        }
    }

    // Enable verbose logging if --verbose provided
    logger.verbose(args.has("verbose"));

    auto app = App();
    return app.run("/etc/passwd");
}
