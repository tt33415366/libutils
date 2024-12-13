#include "coredump.h"
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include "backtrace.h"

static void sig_handler(int sig, siginfo_t *info, void *ucontext)
{
    pid_t pid = fork();
    if (pid != 0) {
	/* Parent */
	// Freeze the original process, unitil its child dump the stack trace
	kill(getpid(), SIGSTOP);
	// Wait for the child without blocking and exit as soon as possible,
	// so that no zombies are left.
	int status;
	waitpid(pid, &status, WNOHANG);
	/* Generating core dump */
	struct sigaction sa;
	sigaction(SIGABRT, NULL, &sa);
	sa.sa_handler = SIG_DFL;
	sigaction(SIGABRT, &sa, NULL);
	abort();
    }
    switch (sig) {
    case SIGSEGV:
	fprintf(stderr, "Got signal %d, faulty address is %p\n", sig, info
		>si_addr);
	break;
    case SIGABRT:
	fprintf(stderr, "Aborted\n");
	break;
    case SIGFPE:
	fprintf(stderr, "Floating point exception\n");
	break;
    default:
	fprintf(stderr, "Caught signal: %d\n", sig);
	break;
    }
    dump_stack();

    /* Dump the memory mappings of the parent process */
    {
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "cat /proc/%d/maps", getppid());
	system(cmd);
    }
    // Resume the parent process
    kill(getppid(), SIGCONT);
    _Exit(EXIT_SUCCESS);
}

void sig_setup()
{
    struct sigaction sa;
    sa.sa_sigaction = sig_handler;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    if (sigaction(SIGSEGV, &sa, NULL) < 0) {
	perror("sigaction(SIGSEGV)");
    }
    if (sigaction(SIGABRT, &sa, NULL) < 0) {
	perror("sigaction(SIGABRT)");
    }
    if (sigaction(SIGFPE, &sa, NULL) < 0) {
	perror("sigaction(SIGFPE)");
    }
}
