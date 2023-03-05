#include "cli.h"

int main(int argc, char *argv[]) {
#if (BINARY == YAopenvfD)
// #ifdef MODE_CLI
    return cli_interface(argc, argv);
#endif
// #else
    return 0;
// #endif
}