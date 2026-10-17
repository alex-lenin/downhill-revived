/* UNIT 0x00159518 0x001595E0 */
typedef struct { char pad[0x2810]; int handles[48]; int ids[20]; int count; } Group;
extern int D_004B2828[];
extern char D_00383A10[];
extern int func_001591D8(unsigned int *);
extern void func_002182A8(int, int);
extern void func_00158120(void *, int, float);
void func_00159518(Group *group, unsigned int *data)
{
    if (D_004B2828[0] < 3) {
        group->handles[group->count] = func_001591D8(data);
        func_002182A8(group->handles[group->count], 0);
        if (data) group->ids[group->count] = (*data >> 7) & 0x7FF;
        func_00158120(D_00383A10, group->handles[group->count], 5.0f);
        group->count++;
    }
}
