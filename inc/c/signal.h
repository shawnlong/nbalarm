/* signal.h standard header */
/* Copyright 2005-2010 IAR Systems AB. */
#ifndef _SIGNAL
#define _SIGNAL

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

_C_STD_BEGIN

_EXTERN_C
                /* type definitions */
typedef void _Sigfun(int);

                /* low-level functions */
__ATTRIBUTES _Sigfun * signal(int, _Sigfun *);

_C_LIB_DECL     /* signal return values */
extern _Sigfun __stm8abi_SIG_DFL;
extern _Sigfun __stm8abi_SIG_IGN;
extern _Sigfun __stm8abi_SIG_ERR;
_END_C_LIB_DECL

_END_EXTERN_C

#if __STM8ABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __STM8ABI_PORTABLE
    #define __STM8ABI_PORTABLE
  #endif

  typedef int sig_atomic_t;

              /* signal codes */
  _C_LIB_DECL
  _DLIB_CONST_ATTR extern int const __stm8abi_SIGABRT;
  _DLIB_CONST_ATTR extern int const __stm8abi_SIGINT;
  _DLIB_CONST_ATTR extern int const __stm8abi_SIGILL;
  _DLIB_CONST_ATTR extern int const __stm8abi_SIGFPE;
  _DLIB_CONST_ATTR extern int const __stm8abi_SIGSEGV;
  _DLIB_CONST_ATTR extern int const __stm8abi_SIGTERM;
  _END_C_LIB_DECL

  #define SIGABRT (_CSTD __stm8abi_SIGABRT)
  #define SIGINT  (_CSTD __stm8abi_SIGINT)
  #define SIGILL  (_CSTD __stm8abi_SIGILL)
  #define SIGFPE  (_CSTD __stm8abi_SIGFPE)
  #define SIGSEGV (_CSTD __stm8abi_SIGSEGV)
  #define SIGTERM (_CSTD __stm8abi_SIGTERM)
  #define _NSIG   32 /* one more than last code */

  #define SIG_DFL (_CSTD __stm8abi_SIG_DFL)
  #define SIG_ERR (_CSTD __stm8abi_SIG_ERR)
  #define SIG_IGN (_CSTD __stm8abi_SIG_IGN)

#else /* __STM8ABI_PORTABILITY_INTERNAL_LEVEL */

  typedef signed char sig_atomic_t;

              /* signal codes */
  #define SIGABRT 22
  #define SIGINT  2
  #define SIGILL  4
  #define SIGFPE  8
  #define SIGSEGV 11
  #define SIGTERM 15
  #define _NSIG   24 /* one more than last code */


                /* signal return values */
  #define SIG_DFL ((_CSTD _Sigfun *)0)
  #define SIG_ERR ((_CSTD _Sigfun *)-1)
  #define SIG_IGN ((_CSTD _Sigfun *)1)
#endif /* __STM8ABI_PORTABILITY_INTERNAL_LEVEL */


_C_LIB_DECL             /* declarations */
__ATTRIBUTES int raise(int);
_END_C_LIB_DECL
_C_STD_END
#endif /* _SIGNAL */

#ifdef _STD_USING
  using _CSTD sig_atomic_t; using _CSTD raise; using _CSTD signal;

  using _CSTD __stm8abi_SIG_DFL; using _CSTD __stm8abi_SIG_IGN; 
  using _CSTD __stm8abi_SIG_ERR;

  #if __STM8ABI_PORTABILITY_INTERNAL_LEVEL
    using _CSTD __stm8abi_SIGABRT; using _CSTD __stm8abi_SIGINT; 
    using _CSTD __stm8abi_SIGILL; using _CSTD __stm8abi_SIGFPE; 
    using _CSTD __stm8abi_SIGSEGV; using _CSTD __stm8abi_SIGTERM;
  #endif
#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
