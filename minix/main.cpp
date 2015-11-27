#include <sys/stat.h>
#include "minix/framework.h"

using namespace ikk;

int main() {
    theFramework = new Framework;

    if (!theFramework->config()) {
        LOG_ERROR << "read config failed";
        return 1;
    }

    if (theFramework->deamon()) {
        if (fork() != 0) {
            return 0;
        }
        umask(0);
        setsid();
    }

    theFramework->start();

    LOG_INFO << "minix exit";
    return 0;
}

