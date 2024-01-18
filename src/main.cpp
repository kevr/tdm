#include <iostream>
#include <format>
using std::cout;
using std::cerr;
using std::endl;

#define PROJECT_NAME "tdm"

int main(int argc, char **argv)
{
    if (argc != 1) {
        cerr << std::format("{} takes no arguments.", argv[0]) << endl;
        return 1;
    }
    cout << std::format("This is project {}.", PROJECT_NAME) << endl;
    return 0;
}
