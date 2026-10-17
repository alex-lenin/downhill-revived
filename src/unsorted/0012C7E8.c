/* UNIT 0x0012C7E8 0x0012C840 */
#include "camera.h"
extern void func_001AE788(void *, void *);

extern void func_001B0410(void *);
extern void func_001B0298(void *);
void func_0012C7E8(char *obj) {
    void *shared = obj + 0x3530;
    func_001AE788(obj + 0x4780, shared);
    func_001AE358((CameraAngles *)(obj + 0x47F0), (RiderCamera *)(shared));
    func_001B0410(obj + 0x4D10);
    func_001B0298(obj + 0x4D40);
}
