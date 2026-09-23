// BismIllahIRRahmaanIRRaheem
/* engine core definitions */

#ifndef FLX_ENGINEDEF_H
#define FLX_ENGINEDEF_H
#ifdef _WIN32
#define FLX_WIN32
#else
#define FLX_NOTWIN32
#endif // _WIN32
#ifdef _MSC_VER
#define FLX_MSVC
#else
#define FLX_NOTMSVC
#endif // _MSC_VER
#ifdef FLX_X64
#define FLX_ACCELERATED
#endif // FLX_X64
#ifdef FLX_RELEASE
#ifdef FLX_MSVC
#define FLX_INLINEFUNC __forceinline
#else
#define FLX_INLINEFUNC inline __attribute__((always_inline))
#endif // FLX_MSVC
#else 
#define FLX_INLINEFUNC
#endif // FLX_RELEASE
#endif // !FLX_ENGINEDEF_H
typedef bool bit;
typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef float float32;
#ifdef FLX_X64
typedef unsigned long long uint64;
typedef long long int64;
typedef double float64;
//-
typedef unsigned long long ulargeint;
typedef long long largeint;
typedef double largefloat;
#else
typedef unsigned int ulargeint;
typedef int largeint;
typedef float largefloat;
#endif // FLX_X64
