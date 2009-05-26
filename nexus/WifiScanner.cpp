#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>

#define LOG_TAG "WifiScanner"
#include <cutils/log.h>

#include "WifiScanner.h"
#include "Supplicant.h"

extern "C" int pthread_cancel(pthread_t thread);

WifiScanner::WifiScanner(Supplicant *suppl, int period) {
    mSuppl = suppl;
    mPeriod = period;
    mActive = false;
}

int WifiScanner::start(bool active) {
    mActive = active;

    if(pipe(mCtrlPipe))
        return -1;

    if (pthread_create(&mThread, NULL, WifiScanner::threadStart, this))
        return -1;
    return 0;
}

void *WifiScanner::threadStart(void *obj) {
    WifiScanner *me = reinterpret_cast<WifiScanner *>(obj);
    me->run();
    pthread_exit(NULL);
    return NULL;
}

int WifiScanner::stop() {
    char c = 0;

    if (write(mCtrlPipe[1], &c, 1) != 1) {
        LOGE("Error writing to control pipe (%s)", strerror(errno));
        return -1;
    }

    void *ret;
    if (pthread_join(mThread, &ret)) {
        LOGE("Error joining to scanner thread (%s)", strerror(errno));
        return -1;
    }

    close(mCtrlPipe[0]);
    close(mCtrlPipe[1]);
    return 0;
}

void WifiScanner::run() {
    LOGD("Starting wifi scanner (active = %d)", mActive);

    while(1) {
        fd_set read_fds;
        struct timeval to;
        int rc = 0;

        to.tv_sec = 0;
        to.tv_sec = mPeriod;

        FD_ZERO(&read_fds);
        FD_SET(mCtrlPipe[0], &read_fds);

        if (mSuppl->triggerScan(mActive)) {
            LOGW("Error triggering scan (%s)", strerror(errno));
        }

        if ((rc = select(mCtrlPipe[0] + 1, &read_fds, NULL, NULL, &to)) < 0) {
            LOGE("select failed (%s) - sleeping for one scanner period", strerror(errno));
            sleep(mPeriod);
            continue;
        } else if (!rc) {
        } else if (FD_ISSET(mCtrlPipe[0], &read_fds))
            break;
    } // while
}
