#ifndef DHD_PARTICLE_UPDATE_H
#define DHD_PARTICLE_UPDATE_H
#ifdef __cplusplus
extern "C" {
#endif
/* Handwritten update consumes the preserved f12 velocity decay multiplier.
 * 0022B708 passes displacement/age scales separately in values, and a vector offset.
 * The six-GPR-only retail declaration hides that seventh input from native C. */
void func_0022B028(void *positions, void *velocity, void *colors, int *count,
                   const float *offset, float *values, float velocityScale);
#ifdef __cplusplus
}
#endif
#endif
