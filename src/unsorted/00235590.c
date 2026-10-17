/* UNIT 0x00235590 0x00235638 */
typedef struct Obj { char pad0[0x604]; char *x604; unsigned int x608; } Obj;
extern char D_00299220[], D_00299248[], D_00299258[];
extern void func_00254E58(char *,int,char *);
int func_00235590(Obj *obj, unsigned int index) {
    if (obj->x604 || (func_00254E58(D_00299220,98,D_00299248), obj->x604)) {
        if (index < obj->x608 || (func_00254E58(D_00299220,102,D_00299258), index < obj->x608)) {
            return *(int *)(obj->x604 + index*0x3928 + 0);
        }
        return 0;
    }
    return 0;
}
