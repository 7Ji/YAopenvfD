#define MODE_CLI
#ifdef MODE_CLI
#include "cli.h"
#endif

int main(int argc, char *argv[]) {
#ifdef MODE_CLI
    return cli_interface(argc, argv);
#else
    return 0;
#endif
}