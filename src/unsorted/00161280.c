/* UNIT 0x00161280 0x001613C8 */
#include "menu.h"
/* Static constructors for the global objects of this C++ file
   (gcc's __static_initialization_and_destruction_0). */
typedef struct Derived {
    char pad0[0x4F8];
    void *vtbl; /* set after the base constructor runs */
} Derived;

extern char D_00386680[], D_00386B90[], D_00387090[], D_00387590[], D_00387A90[];
extern char D_00387F90[], D_00388490[], D_00388990[], D_00389390[], D_00389890[];
extern char D_00389D90[], D_0038A2C0[], D_0038A7D0[], D_0038ACE0[], D_0038B1F0[];
extern char D_0038BC00[], D_0038C100[], D_0038C610[], D_0038CB10[];
extern Derived D_00388E90, D_0038B6F0;
extern char D_00285150[], D_002850C8[];

extern void func_0015CA50(void *);
extern void func_0015CDD8(void *);
extern void func_0015D0C8(void *);
extern void func_0015D4E8(void *);
extern void func_0015DCD8(void *);
extern void func_0015DE00(void *);
extern void func_0015DF30(void *);
extern void func_0015DB78(void *);

extern void func_0015FB58(void *);
extern void func_0015FCB8(void *);
extern void func_0015E6A0(void *);
extern void func_0015FED8(void *);
extern void func_001604D8(void *);
extern void func_00160A10(void *);
extern void func_0015D240(void *);
extern void func_0015E130(void *);
extern void func_0015F5D0(void *);
extern void func_0015FA10(void *);
extern void func_00161110(void *);

void func_00161280(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize) {
        func_0015CA50(D_00386680);
        func_0015CDD8(D_00386B90);
        func_0015D0C8(D_00387090);
        func_0015D4E8(D_00387590);
        func_0015DCD8(D_00387A90);
        func_0015DE00(D_00387F90);
        func_0015DF30(D_00388490);
        func_0015DB78(D_00388990);
        func_0018D9D0((SharedMenu *)(&D_00388E90));
        D_00388E90.vtbl = D_00285150;
        func_0015FB58(D_00389390);
        func_0015FCB8(D_00389890);
        func_0015E6A0(D_00389D90);
        func_0015FED8(D_0038A2C0);
        func_001604D8(D_0038A7D0);
        func_00160A10(D_0038ACE0);
        func_0015D240(D_0038B1F0);
        func_0018D9D0((SharedMenu *)(&D_0038B6F0));
        D_0038B6F0.vtbl = D_002850C8;
        func_0015E130(D_0038BC00);
        func_0015F5D0(D_0038C100);
        func_0015FA10(D_0038C610);
        func_00161110(D_0038CB10);
    }
}
