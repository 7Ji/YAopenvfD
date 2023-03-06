#include "cli.h"

int main(int const argc, char const *const argv[]) {
#if (BINARY == YAopenvfD)
    return cli_interface(argc, argv);
#else
    return 0;
#endif
}