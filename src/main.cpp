// SPDX-License-Identifier: MIT
#include "app.h"
#include "config.h"
#include "lib/curses.h"
#include "util/argparse.h"
#include "util/env.h"
#include "util/filesystem.h"
#include "util/logger.h"
#include "util/termio.h"
#include <sys/stat.h>

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

    // Enable verbose logging if --verbose provided
    logger.verbose(args.has("verbose"));

    auto logdir = tdm_log_dir();
    if (int e = makedirs(logdir); e != 0) {
        return e;
    }

    auto logfile = args.has("log-to")
                       ? std::filesystem::path(args.get("log-to"))
                       : logdir / "tdm.log";

    auto segs = split(logfile, "/");
    segs.pop_back();
    std::filesystem::path current("/");
    for (auto &seg : segs) {
        current /= seg;
        mkdir(current.c_str(), 0755);
    }

    if (!logger.open(logfile)) {
        return tdm::error(1, "unable to open '{}' for writing\n",
                          logfile.c_str());
    } else {
        tdm::print("started logging to '{}'\n", logfile.c_str());
    }

    auto app = App();
    std::string passwd_file(tdm::getenv("TDM_PASSWD", "/etc/passwd"));
    return app.run(passwd_file);
}
