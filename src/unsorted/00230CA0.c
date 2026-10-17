/* UNIT 0x00230CA0 0x00230DA0 .rodata=0x299180:0x2991A0 */
extern char *func_00254544(char *, const char *);
extern int func_00254D68(char *, const char *, ...);
extern char D_00299108[];
extern char D_00299118[];
extern char D_00299128[];
extern char D_00299138[];
extern char D_00299148[];
extern char D_00299158[];
extern char D_00299168[];

void func_00230CA0(char *obj)
{
    char name[64];
    func_00254544(name, obj + 0x8040);
    switch (*(int *)(obj + 0x8088)) {
    case 2:
        func_00254D68(obj + 0x8000, D_00299108, name);
        break;
    case 4:
        func_00254D68(obj + 0x8000, D_00299118, name);
        break;
    case 5:
        func_00254D68(obj + 0x8000, D_00299128, name);
        break;
    case 6:
        func_00254D68(obj + 0x8000, D_00299138, name);
        break;
    case 7:
        func_00254D68(obj + 0x8000, D_00299148, name);
        break;
    case 9:
        func_00254D68(obj + 0x8000, D_00299158, name);
        break;
    default:
        func_00254D68(obj + 0x8000, D_00299168, name);
        break;
    }
}
