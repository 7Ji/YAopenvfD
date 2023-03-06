#include "cli.h"

int main(int argc, char *argv[]) {
#if (BINARY == YAopenvfD)
    return cli_interface(argc, argv);
#else
    return 0;
#endif
}