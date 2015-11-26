#include "minix/framework.h"

using namespace ikk;

Framework* theFramework = NULL;

int main() {
    theFramework = new Framework;

    if (!theFramework->config()) {
        LOG_ERROR << "read config failed";
        return 1;
    }

    theFramework->start();
    return 0;
}

