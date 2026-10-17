/* UNIT 0x001A8918 0x001A8950 */
struct Range {
    char pad0[0xC];
    float max;
    float min;
};

int func_001A8918(struct Range *r, float v)
{
    if (v < r->max && r->min < v) {
        return 1;
    }
    return 0;
}
