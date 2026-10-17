/* UNIT 0x0016B5C0 0x0016B748 */
typedef struct Obj { char pad0[0x5BB4]; int x5BB4; float x5BB8; int x5BBC,x5BC0,x5BC4; float x5BC8,x5BCC,x5BD0,x5BD4,x5BD8; int x5BDC,x5BE0; char pad5BE4[8]; int x5BEC,x5BF0; } Obj;
extern char D_0039E840[],D_0029EA20[],D_0036CFC0[],D_003705C0[],D_003041B0[],D_003BD580[],D_003AD8B0[],D_003B01E0[],D_003C2230[],D_003C42C0[],D_0028AF10[],D_0028AF20[],D_0029C898[],D_0029C8A0[],D_004BD8C0[],D_004F3480[];
extern void func_00230BB8(void *);
extern void func_001182F0(void *,int);
extern void func_00144688(void *,int);
extern void func_00145F40(void *,int);
extern void func_001221A8(void *,int);
extern void func_00181730(void *,int);
extern void func_00172350(void *,int);
extern void func_001742E0(void *,int);
extern void func_00181FE8(void *,int,void *,void *);
extern void func_001DBB00(void *,int);
extern void func_0020EFE8(void *);
void func_0016B5C0(Obj *obj) {
 func_00230BB8(D_0039E840);
 obj->x5BB4=1; obj->x5BDC=8;
 func_001182F0(D_0029EA20,0);
 func_00144688(D_0036CFC0,0);
 func_00145F40(D_003705C0,0);
 func_001221A8(D_003041B0,0);
 func_00181730(D_003BD580,0);
 func_00172350(D_003AD8B0,0);
 func_001742E0(D_003B01E0,0);
 func_00181FE8(D_003C2230,0,D_0028AF10,D_0029C898);
 D_003C2230[0x6E]=0;
 func_00181FE8(D_003C42C0,0,D_0028AF20,D_0029C8A0);
 func_001DBB00(D_004BD8C0,0);
 obj->x5BBC=0; obj->x5BC0=0; obj->x5BC4=0;
 obj->x5BC8=1.09f; obj->x5BCC=0.99f; obj->x5BD0=1.04f; obj->x5BD4=1.0f; obj->x5BD8=1.0f;
 func_0020EFE8(D_004F3480);
 obj->x5BE0=1; obj->x5BB8=6.0f; obj->x5BEC=0; obj->x5BF0=0;
}
