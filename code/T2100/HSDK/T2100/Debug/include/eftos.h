#ifndef __eftos_h
#define __eftos_h

#ifdef __cplusplus
extern "C" {
#endif

#if !defined( __ARMCC_VERSION ) && !defined( __packed )
#define __packed
#endif

#if !defined( __GNUC__ ) && !defined( __attribute__ )
#define __attribute__(A)
#endif

#if defined(MAKE_P2100) || defined(MAKE_H2200)
#include <eftos8.h>
#else
#include <eftos9.h>
#endif

#if (OK == 1)
#define EFT_OK 1
#undef OK
#else
#error "Mismatched definition"
#endif

#if defined(MAKE_P2100) || defined(MAKE_H2200)
#if (COM == RS_3)
#define EFT_COM RS_3
#undef COM
#else
#error "Mismatched definition"
#endif
#else
#if (COM == RS_1)
#define EFT_COM RS_1
#undef COM
#else
#error "Mismatched definition"
#endif
#endif

#if (TIMEOUT == 3)
#define EFT_TIMEOUT 3
#undef TIMEOUT
#else
#error "Mismatched definition"
#endif

#if (CANCEL == 0x80)
#define EFT_CANCEL 0x80
#undef CANCEL
#else
#error "Mismatched definition"
#endif

#if (ENTER == 0x0D)
#define EFT_ENTER 0x0D
#undef ENTER
#else
#error "Mismatched definition"
#endif

#ifdef __cplusplus
}
#endif

#endif // __eftos_h
