#include <sys/types.h>
#define RETSIGTYPE void

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#include <signal.h>
#ifdef HAVE_SIGACTION
#include <string.h>
#endif

//#include "gnuc.h"
#ifdef HAVE_OS_PROTO_H
#include "os-proto.h"
#endif


#ifndef setsignal_h
#define setsignal_h

RETSIGTYPE (*setsignal(int, RETSIGTYPE (*)(int)))(int);
#endif

/*
 * An os independent signal() with BSD semantics, e.g. the signal
 * catcher is restored following service of the signal.
 *
 * When sigset() is available, signal() has SYSV semantics and sigset()
 * has BSD semantics and call interface. Unfortunately, Linux does not
 * have sigset() so we use the more complicated sigaction() interface
 * there.
 *
 * Did I mention that signals suck?
 */
RETSIGTYPE
(*setsignal (int sig, RETSIGTYPE (*func)(int)))(int)		// what is the declaration of "RETSIGTYPE"? ==> (refer to netdump.c, "RETSIGTYPE" = void)
{
#ifdef HAVE_SIGACTION
	struct sigaction old, new;								// what is the exact structure of "struct sigaction"?

	memset(&new, 0, sizeof(new));
	new.sa_handler = func;
#ifdef SA_RESTART
	new.sa_flags |= SA_RESTART;
#endif
	if (sigaction(sig, &new, &old) < 0)						// what is the implementation of "sigaction()" interface (Linux)?
		return (SIG_ERR);
	return (old.sa_handler);

#else
#ifdef HAVE_SIGSET
	return (sigset(sig, func));								// what is the implementation of "sigset()" interface?
#else
	return (signal(sig, func));								// what is the implementation of "signal()" interface?
#endif
#endif
}

