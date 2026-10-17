/* UNIT 0x001AA770 0x001AA7D8 */
typedef struct { int index; int count; float *values; int status; } Sequence;
void func_001AA770(Sequence *p, float *out) {
    int index;
    int count;
    if (p->status != 0) { *(int *)out = 0; return; }
    index = p->index;
    *out = p->values[index];
    index++;
    p->index = index;
    count = p->count;
    if (count < index) { p->status = 2; return; }
    if (index == count && p->status != 2) p->status = 1;
}
