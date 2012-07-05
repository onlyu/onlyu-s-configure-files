
#ifndef _FS_H_
#define _FS_H_

#if defined(__cplusplus)
#define FS_BEGIN_DECLS   extern "C" {
#define FS_END_DECLS     }
#else
#define FS_BEGIN_DECLS
#define FS_END_DECLS
#endif


/* enable the fs depend macros */
#define FS_CTASSERT_ENABLE YES

#if defined(FS_CTASSERT_ENABLE)
#define FS_CTASSERT(x) _FS_CTASSERT(x, __LINE__)
#define _FS_CTASSERT(x, y) __FS_CTASSERT(x, y)
#define __FS_CTASSERT(x, y) typedef char __assert ## y[(x) ? 1 : -1]
#else
#define FS_CTASSERT(x)
#define _FS_CTASSERT(x, y)
#define __FS_CTASSERT(x, y) 
#endif


#endif /*_FS_H_*/
