/* errno.h standard header */
/* Copyright 2003-2010 IAR Systems AB. */
#ifndef _ERRNO
#define _ERRNO

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
_C_STD_BEGIN


#if __STM8ABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __STM8ABI_PORTABLE
    #define __STM8ABI_PORTABLE
  #endif

                /* ERROR CODES */
  _C_LIB_DECL
  _DLIB_CONST_ATTR extern int const __stm8abi_EDOM;
  _DLIB_CONST_ATTR extern int const __stm8abi_ERANGE;
  _DLIB_CONST_ATTR extern int const __stm8abi_EFPOS;
  _DLIB_CONST_ATTR extern int const __stm8abi_EILSEQ;
  _DLIB_CONST_ATTR extern int const __stm8abi_ERRMAX;
  _END_C_LIB_DECL
  #define EDOM    _CSTD __stm8abi_EDOM
  #define ERANGE  _CSTD __stm8abi_ERANGE
  #define EFPOS   _CSTD __stm8abi_EFPOS
  #define EILSEQ  _CSTD __stm8abi_EILSEQ
                /* ADD YOURS HERE */
  #define _NERR   _CSTD __stm8abi_ERRMAX /* one more than last code */
#else /* __STM8ABI_PORTABILITY_INTERNAL_LEVEL */

                /* ERROR CODES */
  #define EDOM    33
  #define ERANGE  34
  #define EFPOS   35
  #define EILSEQ  36
                /* ADD YOURS HERE */
  #define _NERR   37 /* one more than last code */

#endif /* __STM8ABI_PORTABILITY_INTERNAL_LEVEL */


               /* DECLARATIONS */
_C_LIB_DECL
__ATTRIBUTES int volatile *__stm8abi_errno_addr(void);

#if    __STM8ABI_PORTABILITY_INTERNAL_LEVEL \
    || (_MULTI_THREAD && (!_COMPILER_TLS || _GLOBAL_LOCALE))

    #define errno (* (int *) _CSTD __stm8abi_errno_addr())
#else
    _DLIB_DATA_ATTR extern int _TLS_QUAL __iar_Errno;
    #define errno _CSTD __iar_Errno
#endif
_END_C_LIB_DECL


_C_STD_END
#endif /* _ERRNO */

#ifdef _STD_USING
  #ifndef errno
    using _CSTD errno;
  #endif
  #if __STM8ABI_PORTABILITY_INTERNAL_LEVEL
    using _CSTD __stm8abi_EDOM;
    using _CSTD __stm8abi_ERANGE;
    using _CSTD __stm8abi_EFPOS;
    using _CSTD __stm8abi_EILSEQ;
    using _CSTD __stm8abi_ERRMAX;
  #endif
#endif /* _STD_USING */
/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
