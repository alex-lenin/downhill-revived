/* UNIT 0x00119960 0x00119AA0 */
#include "track_bins.h"
typedef struct Item {
    char pad0[0x14];
    unsigned short flags;
} Item;


extern Item *func_001177F8(void *, int);
extern float func_00117ED0(Item *, Item *, int, int);
extern float func_00118138(Item *, Item *, int);

Item *func_00119960(char *obj, int name, float *out) {
    short indices[2];
    Item *from;
    Item *to;
    float value;

    func_00117A78((PathBins *)(obj + 0x10070),(float *)(name),(short *)(&indices[0]),(short *)(&indices[1]));
    to = 0;
    if (indices[0] >= 0) {
        from = func_001177F8(obj, indices[0]);
        if (indices[1] >= 0) {
            to = func_001177F8(obj, indices[1]);
            if (out != 0) {
                value = func_00117ED0(from, to, name, 1);
                *out = value;
                if ((from->flags >> 1) & 1) {
                    if (value > 0.0f)
                        *out += func_00118138(from, to, name);
                    else
                        *out -= func_00118138(from, to, name);
                }
            }
        }
    } else if (indices[1] >= 0) {
        to = func_001177F8(obj, indices[1]);
    }
    return to;
}
