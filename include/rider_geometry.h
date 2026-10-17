#ifndef DHD_RIDER_GEOMETRY_H
#define DHD_RIDER_GEOMETRY_H
#include "vector.h"
typedef struct GroundRider GroundRider;
typedef struct GroundActor GroundActor;
typedef struct GroundNpc GroundNpc;
typedef struct RiderSpeedScale RiderSpeedScale;
typedef struct RiderMarkerHud RiderMarkerHud;

#ifdef __cplusplus
extern "C" {
#endif
float func_00121348(GroundRider *, void *, int);
float func_0012AD80(GroundActor *, void *, int);
float func_0015B910(GroundNpc *, void *, int);
void func_00156568(RiderSpeedScale *, float);
void func_001797A0(RiderMarkerHud *);
void func_0017B848(RiderMarkerHud *, int, int, int, int, float);
float func_00236260(Vector4 *, Vector4 *, Vector4 *, Vector4 *);
float func_002362E8(float *, Vector4 *, Vector4 *, Vector4 *);
float func_00260E08(void *, void *, void *, void *);
#ifdef __cplusplus
}
#endif
#endif
