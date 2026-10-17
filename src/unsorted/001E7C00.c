/* UNIT 0x001E7C00 0x001E7C30 */
typedef struct Entry { char pad[0x4c]; float x4c; int x50; } Entry;
extern Entry D_00272888[];
float func_001E7C00(void *obj, int index) {
    return D_00272888[index].x4c * 0.016666668f;
}
