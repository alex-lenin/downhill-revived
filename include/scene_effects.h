#ifndef DHD_SCENE_EFFECTS_H
#define DHD_SCENE_EFFECTS_H
#ifdef __cplusplus
extern "C" {
#endif
void func_0020ECD0(int red, int green, int blue, float a, float b, float c, float d);
#ifdef __cplusplus
}
#endif
#if !defined(__mips__)
static inline void SceneEffectsLegacy(float a, float b, float c, float d, int red, int green, int blue) {
    func_0020ECD0(red, green, blue, a, b, c, d);
}
#endif
#endif
