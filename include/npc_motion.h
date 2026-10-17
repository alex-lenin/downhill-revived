#ifndef DHD_NPC_MOTION_H
#define DHD_NPC_MOTION_H
#include "scene.h"
/* Prefix used by the NPC proximity/movement state updater. */
typedef struct NpcMotionActor { int x0, state; SceneNode *node; } NpcMotionActor;
typedef struct NpcMotionWorld { char pad0[0x5019C]; float x5019C; } NpcMotionWorld;
#ifdef __cplusplus
extern "C" {
#endif
float func_0015A968(NpcMotionActor *);
#ifdef __cplusplus
}
#endif
#endif
