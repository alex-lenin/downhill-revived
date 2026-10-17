/* UNIT 0x00138900 0x00138998 */
typedef struct { int x0; char pad[0x2BDC]; } LargeItem;
typedef struct { int x0; char pad[0x15FC]; } SmallItem;
typedef struct { char pad0[0x10]; LargeItem large[8]; SmallItem small[16]; char pad1[0x10]; int x2BF20; char pad2[0x2BDC]; int x2EB00; char pad3[0x2BDC]; int *x316E0; } Group;
Group *func_00138900(Group *group)
{
    int i;
    group->x316E0 = &group->x2BF20;
    *group->x316E0 = 0;
    for (i = 7; i >= 0; i--) group->large[i].x0 = 0;
    for (i = 15; i >= 0; i--) group->small[i].x0 = 0;
    group->x2EB00 = 0;
    return group;
}
