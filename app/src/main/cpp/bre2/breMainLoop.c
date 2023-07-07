#include <unistd.h>
#include <sys/syscall.h>
#include <sys/poll.h>
#include <sys/fcntl.h>
#include <AEE_OEMDispatch.h>
#include <pthread.h>

static int gBreMainTimer = -1;
static int gBreMainDispatchPipe[2] = {-1, -1};
static pthread_t gBreMainThread;

int breOemDpyUpdate();

static void *breMainLoop(void *unused) {
    gBreMainTimer = syscall(SYS_timerfd_create, CLOCK_MONOTONIC, 04000);
    if(gBreMainTimer < 0) {
        return NULL;
    }

    if(pipe(gBreMainDispatchPipe) < 0) {
        return NULL;
    }

    fcntl(gBreMainDispatchPipe[0], F_SETFL, fcntl(gBreMainDispatchPipe[0], F_GETFL) | O_NONBLOCK);

    struct pollfd fds[2];
    fds[0].fd = gBreMainTimer;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    fds[1].fd = gBreMainDispatchPipe[0];
    fds[1].events = POLLIN;
    fds[1].revents = 0;

    AEE_Init(0);

    while(1) {
        if (poll(fds, 2, -1) < 0) break;

        if(fds[0].revents & POLLIN) {
            uint64_t buf;
            int expired = read(gBreMainTimer, &buf, sizeof(uint64_t));
            for (int i = 0; i < expired; i++) {
                AEE_Dispatch();
            }
        }
        if(fds[1].revents & POLLIN) {
            char msg;
            while(1) { // discard dispatches while they are there
                ssize_t numRead = read(gBreMainDispatchPipe[0], &msg, 1);
                if(numRead <= 0) {
                    break;
                }
            }
            AEE_Dispatch();
        }
        if(fds[1].revents & POLLHUP) {
            break;
        }

        for(int i = 0; i < 2; i++) {
            fds[i].revents = 0;
        }
    }
    close(gBreMainTimer);
    gBreMainTimer = -1;
    close(gBreMainDispatchPipe[0]);
    gBreMainDispatchPipe[0] = -1;
    AEE_Exit();

    return NULL;
}

void OEMOS_SetTimer(uint32 nMSecs) {
    struct itimerspec its;
    memset(&its, 0, sizeof(struct itimerspec));
    its.it_value.tv_sec = nMSecs / 1000;
    its.it_value.tv_nsec = (nMSecs % 1000) * 1000000;

    syscall(SYS_timerfd_settime, gBreMainTimer, 0, &its, NULL);
}

void OEMOS_SignalDispatch(void) {
    char msg = 1;
    write(gBreMainDispatchPipe[1], &msg, 1);
}

void OEMOS_CancelDispatch(void) {
    // not possible to implement with android looper
}

void breMainStart() {
    pthread_create(&gBreMainThread, NULL, breMainLoop, NULL);
}

void breMainTerminate() {
    if(gBreMainDispatchPipe[1] != -1) {
        int pip = gBreMainDispatchPipe[1];
        gBreMainDispatchPipe[1] = -1;
        close(pip);

        pthread_join(gBreMainThread, NULL);
    }
}