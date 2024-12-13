#include "backtrace.h"
#ifndef __USE_GNU
#define __USE_GNU /* g++ will define the __USE_GNU macro for us, while gcc won't */
#endif /* __USE_GNU */
#include <execinfo.h>
#include <ucontext.h>
#include <unistd.h>

void dump_stack()
{
    void *trace[64];
    int nptrs = backtrace(trace, sizeof(trace) / sizeof(trace[0]));
    ucontext_t *uc = (ucontext_t *)ucontext;
    // Overwirte sigaction with caller's address
#ifdef __linux__
#if defined(__arm__)
    trace[1] = (void *)uc->uc_mcontext.arm_pc;
#elif defined(__aarch64__)
    trace[1] = (void *)uc->uc_mcontext.pc;
#elif defined(__x86_64__)
    trace[1] = (void *)uc->uc_mcontext.gregs[REG_RIP];
#elif defined(__i386__)
    trace[1] = (void *)uc->uc_mcontext.gregs[REG_EIP];
#elif (defined(__mips__) || defined(__mips64__))
    trace[1] = (void *)uc->uc_mcontext.sc_pc;
#else
#error Only ARM, AARCH64, X86, X86-64, mips and mips64 are supported
#endif
#endif
    backtrace_symbols_fd(&trace[1], nptrs - 1, STDERR_FILENO);
}
