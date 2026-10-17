/* UNIT 0x001292E0 0x00129330 .rodata=0x288F70:0x288FA0 */
typedef struct Obj { char pad[0x28]; int x28; char pad2c[0x170]; int x19c; } Obj;
int func_001292E0(Obj *obj) {
 switch(obj->x28) {
 case 2: case 3: case 4: return obj->x19c>=6;
 case 5: return 1;
 case 0: case 1: case 6: case 7: case 8: case 9: default: return 0;
 }
}
