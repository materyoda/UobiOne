// base.h
#ifndef _BASE_H_
#define _BASE_H_

#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <stdio.h>
#include <string>

//-----------------------------------------------------------------------------
// Basic types
typedef unsigned       char byte;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

//-----------------------------------------------------------------------------
// Debug
#define DEBUG 1

//-----------------------------------------------------------------------------
// Useful macros
#define SIZE_ARRAY(__a__) (sizeof(__a__)/sizeof(__a__[0]))
#define LOG(ALL_ARGS) printf ALL_ARGS
#define TRACE(x) do { if (DEBUG) printf x; } while (0)

//#define LOG(ALL_ARGS)
#define SAFESUB(ARG_BASE, ARG_CHUNK)          (ARG_BASE - ARG_CHUNK > 0        ? ARG_BASE - ARG_CHUNK : 0)
#define SAFEADD(ARG_BASE, ARG_CHUNK, ARG_MAX) (ARG_BASE + ARG_CHUNK <= ARG_MAX ? ARG_BASE + ARG_CHUNK : ARG_MAX)
#define MAX(ARG_A, ARG_B) ((ARG_A)>(ARG_B)?(ARG_A):(ARG_B))
#define MIN(ARG_A, ARG_B) ((ARG_A)<(ARG_B)?(ARG_A):(ARG_B))

//-----------------------------------------------------------------------------
struct ivec2 { int x, y; };

//-----------------------------------------------------------------------------
struct vec2 { float x, y; };
inline vec2  vmake(float x, float y) { vec2 r; r.x = x; r.y = y; return r; }
inline vec2  vadd(vec2 v1, vec2 v2) { return vmake(v1.x + v2.x, v1.y + v2.y); }
inline vec2  vsub(vec2 v1, vec2 v2) { return vmake(v1.x - v2.x, v1.y - v2.y); }
inline vec2  vscale(vec2 v, float f) { return vmake(v.x * f, v.y * f); }
inline float vlen2(vec2 v) { return v.x * v.x + v.y * v.y; }
inline float vlen(vec2 v) { return (float)sqrt(vlen2(v)); }
inline float vdot(vec2 v1, vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }
inline vec2  vunit(float angle) { return vmake((float)cos(angle), (float)sin(angle)); }
inline vec2  vunit(vec2 v) { return vscale(v, 1.f / vlen(v)); }
inline s32   vcomp(vec2 v1, vec2 v2) { return 0 == vlen2(v1) - vlen(v2) == 0; }

#endif
