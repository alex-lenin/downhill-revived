/* UNIT 0x00176278 0x00176468 */
typedef struct List { char pad0[8]; unsigned short count; char pada[0x16]; int *items[1]; } List;
typedef struct Sound { int x0,x4,x8,xc; } Sound;
typedef struct Obj { char pad[0xf8c]; List *xf8c,*xf90; char padf94[0x30]; Sound xfc4[9],x1054[9],x10e4,x10f4,x1104,x1114,x1124,x1134; } Obj;
typedef struct Global { char pad0[0x288]; int x288; char pad28c[0xcc]; int x358; } Global;
extern Global D_004B25A0;
extern void func_00203590(Sound *,int,int,int);
void func_00176278(Obj *obj) {
 int i,id,mask; Sound *sound,*sound2; int *flags;
 if(obj->xf8c) {
 id=0x1fb8; if(D_004B25A0.x288>=2) id=0x1fc2;
 i=0;
 if(obj->xf8c->count) { mask=~0x3f; sound=obj->xfc4; do {
 if(i<9) func_00203590(sound,id+1,i,0);
 if(i>=D_004B25A0.x358) { obj->xf8c->items[i][0]=(obj->xf8c->items[i][0]&mask)|9; }
 i++; sound++;
 } while(i<obj->xf8c->count); }
 func_00203590(&obj->x10e4,id,0,0);
 if(D_004B25A0.x288>=2) {
 func_00203590(&obj->x10f4,id,1,0);
 if(D_004B25A0.x288>=2) {
 func_00203590(&obj->x1124,0x1fcc,0,0);
 i=0; func_00203590(&obj->x1134,0x1fcc,1,0);
 if(obj->xf90->count) { sound2=obj->x1054; do {
 if(i<9) func_00203590(sound2,0x1fcd,i,0);
 if(i>=D_004B25A0.x358) { obj->xf90->items[i][0]=(obj->xf90->items[i][0]&~0x3f)|9; }
 i++; sound2++;
 } while(i<obj->xf90->count); }
 }
 }
 }
}
