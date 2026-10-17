/* UNIT 0x00223BB0 0x00224498 .sdata=0x29DAFC:0x29DB30 */
#include "include_asm.h"

int D_0029DAFC = 0;
int D_0029DB00 = 1;
int D_0029DB04 = 0;
int D_0029DB08 = 0;
int D_0029DB0C = 0x015D0000;
int D_0029DB10 = 0x01B48000;
int D_0029DB14 = 0x01DE3350;
int D_0029DB18 = 0x01E635D0;
int D_0029DB1C = 0x01EBD6D0;
int D_0029DB20 = 1;
int D_0029DB24 = 0;
int D_0029DB28 = 0;
unsigned char D_0029DB2C = 0;

extern int D_0029DC1C;
extern int D_0029E2C4;
extern int D_0029E2C8;
extern int D_006754B8[];
extern char D_00663B40[];
extern char D_00675480[];
extern char D_006754E8[];
extern char D_00298EA0[];

extern void func_00201D50(unsigned int);
extern void func_0023E000(void *);
extern void func_0025C7D0(int);
extern void func_0020B938(void *, int, int);
extern void func_002240A8(int);
extern void func_00223C60(int);
extern void func_0025A590(int);
extern int func_002424C8(void *);
extern int func_00254D08(void *, int);
extern void func_00242540(void *);

int func_00223BB0(int arg0, int arg1)
{
    func_00201D50(0xFFFFFDFF);
    D_0029DB20 = arg1;
    D_0029DC1C = 0;
    func_0023E000(D_00663B40);
    func_0025C7D0(0);
    func_0020B938(D_00675480, 0x280, 0xE0);
    func_002240A8(arg0);
    D_0029DB24 = 0;
    D_0029E2C8 = D_006754B8[0];
    D_0029E2C4 = D_006754B8[0];
    D_0029DB28 = 0;
    D_0029DB2C = 0;
    func_00223C60(1);
    D_0029DAFC = 1;
    return D_006754B8[0];
}

struct State { char pad[0xB0]; int unkB0,unkB4; };
extern struct State D_00663B58;
extern int D_0029E2C4;
extern int D_0029E2C8;
extern int func_0023C288(void *);
extern int func_0023C308(void *);
extern int func_0023C3D8(void *);
extern void func_0023C278(void *);
extern void func_0020C090(void);
extern void func_0020C050(int);
extern void func_00242328(void *);
extern void func_002424E0(void *);
extern int func_00224308(void);
extern void func_00224058(void);
extern int func_00201980(int);
extern void func_00201948(int, int);
extern int func_002013F8(void);
extern int func_00230618(int, int *);
extern int func_00236928(void *, int, int);
extern int func_00230648(int, int);
extern int func_00230690(int, int *);
extern int func_0024C338(void *, int, int, int, int);
extern int func_002306D8(int, int);
extern int func_00224410(void);
extern int func_0023E018(void *);
extern int func_00224078(void);

void func_00223C60(int arg0)
{
    int volume;
    int level;
    int size;
    int done;
    int size2;
    int done2;
    int src;
    int buf;

    if (func_0023C288(&D_00663B58) == 3) {
        func_0020C090();
        if (D_0029DB20) {
            func_00242328(D_006754E8);
        }
        func_00224308();
        return;
    }
    if (arg0 == 0) {
        if (D_0029DB24) {
            volume = func_00201980(0x10);
            level = volume;
            while (level > 0) {
                level -= 100;
                if (level < 0) {
                    level = 0;
                }
                func_00201948(0x10, level);
                while (func_002013F8());
            }
            func_0023C278(&D_00663B58);
            while (!func_0023C308(&D_00663B58)) {
                func_00224058();
            }
            while (!func_0023C3D8(&D_00663B58) && func_0023C288(&D_00663B58) != 3) {
                func_00224058();
            }
            func_0020C090();
            func_00224308();
            func_00201948(0x10, volume);
            while (func_002013F8());
            return;
        }
        func_0020C050(1);
        if (D_0029DB20) {
            func_002424E0(D_006754E8);
        }
        D_0029DB24 = 1;
        if (!func_0023C3D8(&D_00663B58) && func_0023C288(&D_00663B58) != 3) {
            func_00224058();
        }
        func_0023C278(&D_00663B58);
    }
    if (D_0029E2C8 >= 5 && func_0023C288(&D_00663B58) != 3) {
        size = func_00230618(D_0029DB18, &src);
        if (D_0029E2C4 > 0 && size > 0xFFFF) {
            done = func_00236928(D_006754B8, src, 0x10000);
            func_00230648(D_0029DB18, done);
            D_0029E2C4 -= done;
        }
        func_00224058();
        size2 = func_00230690(D_0029DB18, &buf);
        if (size2 > 0) {
            done2 = func_0024C338(&D_00663B58, buf, size2, D_0029DB18, *(int *)(D_0029DB18 + 0x50008));
            func_002306D8(D_0029DB18, done2);
            D_0029E2C8 -= done2;
        }
        if (D_0029DB20) {
            func_00224410();
        }
        if (D_0029DB24 == 0 && func_0023E018(D_00663B40) && func_00224078()) {
            func_0020C050(1);
            if (D_0029DB20) {
                func_002424E0(D_006754E8);
            }
            D_0029DB24 = 1;
        }
        return;
    }
    if (D_0029DB24 == 0) {
        func_0020C050(1);
        if (D_0029DB20) {
            func_002424E0(D_006754E8);
        }
        D_0029DB24 = 1;
    } else if (D_0029DB2C == 0) {
        D_0029DB2C = 1;
        while (!func_0023C308(&D_00663B58) && func_0023C288(&D_00663B58) != 2) {
            func_00224058();
        }
    }
    if (!func_0023C3D8(&D_00663B58) && func_0023C288(&D_00663B58) != 3) {
        func_00224058();
    }
}


INCLUDE_ASM("asm/nonmatchings/text", func_00224050);

void func_00224058(void)
{
    func_0025A590(1);
}

int func_00224078(void)
{
    int result;

    if (D_0029DB20 == 0) {
        result = 1;
    } else {
        result = func_002424C8(D_006754E8);
    }
    return result;
}

extern int D_0029D9F0,D_0029D9F4,D_0029D9FC,D_0029E2CC,D_0029E2D0;
extern struct State D_00663B58;
typedef struct { int x0; void (*x4)(void); void *x8; int xC; void *x10; int x14; int x18,x1C,x20; } Thread;
extern Thread D_00675540,D_00675570;
extern char D_00663C40[],D_006755A0[],D_00669480[],D_00664C80[],D_00665480[];
extern int _gp;
extern void func_002302C0(void),func_002303E8(void),func_002243C0(void),func_0023C420(void);
extern int func_0020BED0(void),func_0020C018(void);
extern void func_002305F0(int,int),func_0024CE98(void);
extern void func_0023C0C8(void *,int,int,int,void *,int,void *,int);
extern void func_002422B0(void *,void *,int,int);
extern int func_0023C1D0(void *,int,int,void (*)(void),int);
extern void func_0023DFA8(void *,int,int,int);
extern int func_0025A4E0(Thread *);
extern int func_0025A500(int,void *);
extern int func_00236700(void *,int);
extern void func_0025F298(int);
extern int func_0025A3C0(int,int (*)(void),int),func_0025A3F0(int,int (*)(void),int);
extern void func_0025AF98(int),func_0025B068(int);
void func_002240A8(int value) {
 int id;
 D_0029DB00=0;
 *(volatile int *)0x1000E000 |=3;
 *(volatile int *)0x1000E010=4;
 func_002305F0(D_0029DB18,0x50000);
 func_0024CE98();
 func_0023C0C8(&D_00663B58,D_0029DB1C,0x13C76C,D_0029DB14,D_00663C40,256,D_006755A0,512);
 func_002422B0(D_006754E8,D_00669480,0xC000,0x6000);
 func_0023C1D0(&D_00663B58,0,0,func_002302C0,D_0029DB18);
 if(D_0029DB20) func_0023C1D0(&D_00663B58,2,0,func_002303E8,D_0029DB18);
 func_0023DFA8(D_00663B40,(D_0029DB0C&0xFFFFFFF)|0x20000000,D_0029DB10,5);
 D_0029D9F0=0x8C000;
 D_0029D9F4=640;
 D_00675540.x4=func_002243C0;
 D_00675540.x8=D_00664C80;
 D_00675540.xC=0x800;
 D_00675540.x14=1;
 D_00675540.x10=&_gp;
 D_00675540.x20=0;
 id=func_0025A4E0(&D_00675540);
 D_0029E2D0=id;
 func_0025A500(id,0);
 D_00675570.x4=func_0023C420;
 D_00675570.x8=D_00665480;
 D_00675570.xC=0x4000;
 D_00675570.x10=&_gp;
 D_00675570.x14=1;
 D_00675570.x20=0;
 id=func_0025A4E0(&D_00675570);
 D_0029E2CC=id;
 func_0025A500(id,&D_00663B58);
 if(!func_00236700(D_006754B8,value)) func_0025F298(1);
 D_00663B58.unkB4=func_0025A3C0(2,func_0020BED0,0);
 func_0025AF98(2);
 D_0029D9FC=1;
 D_00663B58.unkB0=func_0025A3F0(2,func_0020C018,0);
 func_0025B068(2);
}


extern int D_0029D9F8;
extern int D_0029D9FC;
extern int D_0029E2CC;
extern int D_0029E2D0;
extern struct State D_00663B58;

extern void func_00230610(int);
extern int func_002368E8(void *);
extern void func_0023C240(void *);
extern void func_0023DFF8(void *);
extern void func_00242370(void *);
extern void func_0025A3E0(int, int);
extern void func_0025A410(int, int);
extern void func_0025A4F0(int);
extern void func_0025A530(int);
extern void func_0025AF30(int);
extern void func_0025B000(int);

int func_00224308(void)
{
    int result;
    func_00230610(D_0029DB18);
    func_0023DFF8(D_00663B40);
    func_0025A530(D_0029E2CC);
    func_0025A4F0(D_0029E2CC);
    func_0025A530(D_0029E2D0);
    func_0025A4F0(D_0029E2D0);
    func_0025B000(2);
    func_0025A410(2, D_00663B58.unkB0);
    func_0025AF30(2);
    func_0025A3E0(2, D_00663B58.unkB4);
    D_0029D9FC = 0;
    func_0023C240(&D_00663B58);
    func_00242370(D_006754E8);
    result = func_002368E8(D_006754B8);
    D_0029D9F8 = 0;
    D_0029DB00 = 1;
    D_0029DAFC = 0;
    return result;
}

INCLUDE_ASM("asm/nonmatchings/text", func_002243C0);

int func_002243E8(int value)
{
    return func_00254D08(D_00298EA0, value);
}

int func_00224410(void)
{
    func_00242540(D_006754E8);
    return 1;
}

void func_00224438(int address)
{
    *(volatile int *)0x1000A030 = address & 0x0FFFFFFF;
    *(volatile int *)0x1000A020 = 0;
    *(volatile int *)0x1000A000 = 0x105;
    if (D_0029DB04 == 0) {
        D_0029DB04 = 1;
        return;
    }
    if (D_0029DB08 == 0) {
        D_0029DB08 = 1;
    }
}
