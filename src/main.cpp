#include "app.h"
#include <format>
#include <iostream>
using std::cerr;
using std::endl;

#define PROJECT_NAME "tdm"

int main(int argc, char **argv)
{
    if (argc != 1) {
        cerr << std::format("{} takes no arguments.", argv[0]) << endl;
        return 1;
    }
    auto app = App();
    return app.run();
}
