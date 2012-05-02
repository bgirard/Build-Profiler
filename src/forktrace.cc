#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
#include <unistd.h>
#include <time.h>
#include <QuartzCore/QuartzCore.h>

#define PREFIX "libmtrace func=%10s: "

#define RENDERTRACE

extern int __argc;
extern char ** __argv;

int currentMilli() {
    return 1000 * CACurrentMediaTime();
}

#ifdef RENDERTRACE
void RenderTraceEventStart(const char *aInfo, pid_t id)
{
    fprintf(stderr, "%s RENDERTRACE %i fillrect #%08x 0 0 10 10\n", aInfo, (int)currentMilli(), (int)id);
}

void RenderTraceEventEnd(const char *aInfo, pid_t id)
{
    fprintf(stderr, "%s RENDERTRACE %i fillrect #%08x 0 0 0 0\n", aInfo, (int)currentMilli(), (int)id);
}
#endif

pid_t fork(void) {
    pid_t start_pid = getpid();

    static pid_t (*real_fork)(void) = NULL;
    if (!real_fork)
        real_fork = (pid_t (*)(void))dlsym(RTLD_NEXT, "fork");

    pid_t p = real_fork();
    if( p != 0 ) {
        fprintf(stderr, PREFIX "%i forked to -> %i\n", __func__, start_pid, p);
#ifdef RENDERTRACE
//        RenderTraceEventStart("fork", start_pid);
#endif
    }
    return p;
}

int execve(const char *filename, char *const argv[], char *const envp[]) {
    static int (*real_execve)(const char*, char *const*, char* const*) = NULL;
    if (!real_execve)
        real_execve = (int (*)(const char*, char *const*, char* const*))dlsym(RTLD_NEXT, "execve");

#ifdef RENDERTRACE
    RenderTraceEventStart(filename, getpid());
#endif
    fprintf(stderr, PREFIX "execve called by pid=%i %s at %i\n", __func__, getpid(), filename, currentMilli());
    for( int i = 0; argv[i]; i++ ) {
        fprintf(stderr, PREFIX "  ARGV[%i] = %s\n", __func__, i, argv[i]);
    }
    int p = real_execve(filename, argv, envp);
    return p;
}

void exit(int exit_code) {
#ifdef RENDERTRACE
    RenderTraceEventEnd("exit", getpid());
#endif
    fprintf(stderr, PREFIX "end of pid=%i at %i\n", __func__, getpid(), currentMilli());
    static void (*real_exit)(int);
    if (!real_exit)
        real_exit = (void (*)(int))dlsym(RTLD_NEXT, "exit");

    real_exit(exit_code);
}



/*
void* malloc(size_t size)
{
    static void* (*real_malloc)(size_t) = NULL;
    if (!real_malloc)
        real_malloc = (void* (*)(size_t))dlsym(RTLD_NEXT, "malloc");

    void *p = real_malloc(size);
    fprintf(stderr, "malloc(%d) = %p\n", size, p);
    return p;
}*/

