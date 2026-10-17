#ifndef DHD_SCALAR_API_H
#define DHD_SCALAR_API_H
/* EE DI scalars occupy one GPR; these aliases keep their native 64-bit width. */
typedef long ScalarSigned64 __attribute__((mode(DI)));
typedef unsigned long ScalarUnsigned64 __attribute__((mode(DI)));
typedef void (*ScalarCompletionCallback)(int, ScalarUnsigned64);
#if !defined(__mips__)
struct AnimationHandle;
struct Obj001B3D58;
struct Obj001CFA00;
struct Obj001D00F0;
struct Scalar00139940Obj;
struct Scalar00169F50Obj;
struct Scalar00169F98Obj;
struct Scalar00169FF0Obj;
struct Scalar0016A010Obj;
struct Scalar0016C9E8Obj;
struct Scalar0017ED50Obj;
struct Scalar0017F3F0Obj;
struct Scalar0017F3F0Entry;
struct Scalar00185730Input;
struct Scalar00185730Obj;
struct Scalar00186610Input;
struct Scalar00186610Obj;
struct Scalar0018D898Obj;
struct Scalar001C2458Obj;
struct Scalar001F97A8Obj;
struct Scalar00231E00Obj;
struct Scalar00231E00Vec;
struct Scalar00239540Obj;
#ifdef __cplusplus
extern "C" {
#endif
float func_00126D90(float, float);
void func_00139940(struct Scalar00139940Obj *, float *, float, int);
void func_0013CA48(char *, int, void *);
void func_001554B0(void);
void func_001554B8(void);
void func_00169F50(struct Scalar00169F50Obj *, float);
void func_00169F98(struct Scalar00169F98Obj *, float);
void func_00169FF0(struct Scalar00169FF0Obj *, float);
int func_0016A010(struct Scalar0016A010Obj *);
int func_0016C9E8(struct Scalar0016C9E8Obj *, int);
int func_0017ED50(struct Scalar0017ED50Obj *, int, int);
struct Scalar0017F3F0Entry * func_0017F3F0(struct Scalar0017F3F0Obj *, int);
void func_00185730(struct Scalar00185730Obj *, struct Scalar00185730Input *);
void func_00186610(struct Scalar00186610Obj *, struct Scalar00186610Input *);
void func_0018D898(struct Scalar0018D898Obj *, short, int);
void func_001B3D58(struct Obj001B3D58 *, int, float);
void func_001B5AF0(unsigned char *, int, int);
void func_001C2458(struct Scalar001C2458Obj **, struct Scalar001C2458Obj **, void *, void *, void *, void *, int, int, float, float, unsigned int);
void func_001CFA00(struct Obj001CFA00 *, int);
void func_001D00F0(struct Obj001D00F0 *, int);
int func_001F97A8(struct Scalar001F97A8Obj *, int, int);
void func_00202688(int, int, int, int, int, int, int, int, int, int, ScalarCompletionCallback, ScalarUnsigned64);
void func_00203380(const struct AnimationHandle *, void *);
void func_00208C50(int, ScalarUnsigned64);
void func_0020B908(const void *, const void *, float);
void * func_0020BD18(void *, ScalarSigned64, ScalarSigned64, ScalarSigned64);
void func_0021E3A8(void);
void func_00231E00(struct Scalar00231E00Obj *, struct Scalar00231E00Obj *, struct Scalar00231E00Vec *, struct Scalar00231E00Vec *, float);
void func_00235E18(int, ScalarUnsigned64);
void func_00235E70(int, ScalarUnsigned64);
void func_00239540(struct Scalar00239540Obj *, float *, float *, float *, float *, unsigned char, int);
void func_00239A80(float, float, float, float);
void func_00239A98(float);
void func_0023B220(ScalarUnsigned64 *, ScalarUnsigned64, ScalarUnsigned64, ScalarUnsigned64);
void func_0023D4F8(ScalarSigned64, ScalarSigned64, ScalarSigned64, ScalarSigned64);
#ifdef __cplusplus
}
#endif
static inline float ScalarBridge00126D90_0(const void * p0, float p1, float p2) { return (float)func_00126D90((float)p1, (float)p2); }
static inline void ScalarBridge001C2458_1(const void * p0, const void * p1, int p2, const void * p3, const void * p4, const void * p5, ScalarUnsigned64 p6, float p7, float p8, unsigned short p9, int p10) { func_001C2458((struct Scalar001C2458Obj **)p0, (struct Scalar001C2458Obj **)p1, (void *)p2, (void *)p3, (void *)p4, (void *)p5, (int)p6, (int)p9, (float)p7, (float)p8, (unsigned int)p10); }
static inline void ScalarBridge00231E00_2(const void * p0, const void * p1, const void * p2, const void * p3, float p4, float p5, float p6) { func_00231E00((struct Scalar00231E00Obj *)p0, (struct Scalar00231E00Obj *)p1, (struct Scalar00231E00Vec *)p2, (struct Scalar00231E00Vec *)p3, (float)p4); }
static inline void ScalarBridge00139940_3(const void * p0, const void * p1, int p2, int p3, float p4) { func_00139940((struct Scalar00139940Obj *)p0, (float *)p1, (float)p4, (int)p2); }
static inline void ScalarBridge0013CA48_4(const void * p0, ScalarSigned64 p1, const void * p2) { func_0013CA48((char *)p0, (int)p1, (void *)p2); }
static inline int ScalarBridge001F97A8_5(const void * p0, int p1, int p2) { return (int)func_001F97A8((struct Scalar001F97A8Obj *)p0, (int)p1, (int)p2); }
static inline void ScalarBridge001554B8_6(const void * p0, float p1) { func_001554B8(); }
static inline void ScalarBridge001554B0_7(const void * p0, float p1, float p2) { func_001554B0(); }
static inline void ScalarBridge001CFA00_8(const void * p0, ScalarSigned64 p1) { func_001CFA00((struct Obj001CFA00 *)p0, (int)p1); }
static inline void ScalarBridge001B5AF0_9(const void * p0, ScalarSigned64 p1, ScalarSigned64 p2) { func_001B5AF0((unsigned char *)p0, (int)p1, (int)p2); }
static inline void ScalarBridge00239540_10(const void * p0, const void * p1, const void * p2, const void * p3, const void * p4, unsigned int p5, ScalarSigned64 p6) { func_00239540((struct Scalar00239540Obj *)p0, (float *)p1, (float *)p2, (float *)p3, (float *)p4, (unsigned char)p5, (int)p6); }
static inline void ScalarBridge00169F50_11(const void * p0, float p1, float p2) { func_00169F50((struct Scalar00169F50Obj *)p0, (float)p1); }
static inline void ScalarBridge00169F98_12(const void * p0, float p1, float p2) { func_00169F98((struct Scalar00169F98Obj *)p0, (float)p1); }
static inline void ScalarBridge00169FF0_13(const void * p0, float p1, float p2) { func_00169FF0((struct Scalar00169FF0Obj *)p0, (float)p1); }
static inline int ScalarBridge0016A010_14(const void * p0, float p1) { return (int)func_0016A010((struct Scalar0016A010Obj *)p0); }
static inline int ScalarBridge0016C9E8_15(const void * p0, const void * p1, float p2) { return (int)func_0016C9E8((struct Scalar0016C9E8Obj *)p0, (int)p1); }
static inline int ScalarBridge0017ED50_16(const void * p0, int p1, int p2) { return (int)func_0017ED50((struct Scalar0017ED50Obj *)p0, (int)p1, (int)p2); }
static inline int ScalarBridge0017F3F0_17(const void * p0, int p1, float p2) { return (int)func_0017F3F0((struct Scalar0017F3F0Obj *)p0, (int)p1); }
static inline void * ScalarBridge0017F3F0_18(const void * p0, const void * p1, float p2) { return (void *)func_0017F3F0((struct Scalar0017F3F0Obj *)p0, (int)p1); }
static inline void ScalarBridge00185730_19(ScalarSigned64 p0, ScalarSigned64 p1) { func_00185730((struct Scalar00185730Obj *)p0, (struct Scalar00185730Input *)p1); }
static inline void ScalarBridge00186610_20(ScalarSigned64 p0, ScalarSigned64 p1) { func_00186610((struct Scalar00186610Obj *)p0, (struct Scalar00186610Input *)p1); }
static inline void ScalarBridge0018D898_21(const void * p0, ScalarSigned64 p1, const void * p2) { func_0018D898((struct Scalar0018D898Obj *)p0, (short)p1, (int)p2); }
static inline void ScalarBridge001D00F0_22(const void * p0, int p1, float p2) { func_001D00F0((struct Obj001D00F0 *)p0, (int)p1); }
static inline void ScalarBridge00239A80_23(const void * p0, float p1, float p2, float p3, float p4) { func_00239A80((float)p1, (float)p2, (float)p3, (float)p4); }
static inline void ScalarBridge00239A98_24(const void * p0, float p1) { func_00239A98((float)p1); }
static inline void ScalarBridge00203380_25(const void * p0, const void * p1, float p2) { func_00203380((const struct AnimationHandle *)p0, (void *)p1); }
static inline void ScalarBridge001B3D58_26(const void * p0, int p1, int p2, float p3) { func_001B3D58((struct Obj001B3D58 *)p0, (int)p1, (float)p3); }
static inline void ScalarBridge0023D4F8_27(ScalarSigned64 p0, ScalarSigned64 p1, ScalarSigned64 p2, int p3) { func_0023D4F8((ScalarSigned64)p0, (ScalarSigned64)p1, (ScalarSigned64)p2, (ScalarSigned64)p3); }
static inline void ScalarBridge0023D4F8_28(int p0, int p1, int p2, int p3) { func_0023D4F8((ScalarSigned64)p0, (ScalarSigned64)p1, (ScalarSigned64)p2, (ScalarSigned64)p3); }
static inline void ScalarBridge0020B908_29(const void * p0, const void * p1, unsigned int p2, float p3) { func_0020B908((const void *)p0, (const void *)p1, (float)p3); }
static inline void * ScalarBridge0020BD18_30(const void * p0, int p1, int p2, int p3) { return (void *)func_0020BD18((void *)p0, (ScalarSigned64)p1, (ScalarSigned64)p2, (ScalarSigned64)p3); }
static inline void ScalarBridge0021E3A8_31(float p0) { func_0021E3A8(); }
static inline void ScalarBridge0023B220_32(unsigned int p0, unsigned int p1, int p2, int p3) { func_0023B220((ScalarUnsigned64 *)p0, (ScalarUnsigned64)p1, (ScalarUnsigned64)p2, (ScalarUnsigned64)p3); }
#endif
#endif
