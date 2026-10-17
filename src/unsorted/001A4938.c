/* UNIT 0x001A4938 0x001A4C28 */
#include "format.h"
typedef struct { char pad0[0x18]; int x18; char pad1C[0x14]; int x30,x34,x38; } Menu;
typedef struct { char pad0[0x1C]; int x1C; } State;
extern Menu D_004C3740;
extern State D_0043EEC0;
extern int D_003F52F0[];
extern char D_003B2270[];
extern char D_0028E7F0[],D_0028E808[],D_0028E820[],D_0028E838[],D_0028E848[],D_0028E858[],D_0028E868[],D_0028E878[];
extern void func_0017A678(void *,int);
extern void func_0023D650(int,int *,int *);
extern void func_0020F648(int,int,int);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,char *);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,char *);

static inline int *GetValue(State *state) { return &state->x1C; }
static inline void *GetChild(char *ptr) { return ptr+0x150; }
void func_001A4938(int arg0,int alpha) {
    char text[48];
    int centerX,screenY,view;
    int *value;
    value=GetValue(&D_0043EEC0);
    view=D_003F52F0[0];
    func_0017A678(GetChild(D_003B2270),-2);
    func_0023D650(view,&centerX,&screenY);
    centerX/=2;
    func_0020F648(0,36,24);
    func_0020F518(0,200,200,0,alpha);
    func_0020F9F0(0,view,centerX,5,D_0028E7F0);
    func_0020FB80(0,336,288,-98,-32);
    if (D_004C3740.x38==0) func_0020F9F0(0,view,centerX,30,D_0028E808);
    else if (D_004C3740.x38==1) func_0020F9F0(0,view,centerX,30,D_0028E820);
    else func_0020F9F0(0,view,centerX,30,D_0028E838);
    func_0020F518(0,110,110,200,alpha);
    func_0020F9A8(0,view,120,60,D_0028E848);
    func_0020F518(0,88,152,73,alpha);
    FormatHmsDiscard(*value, (char *)(text));
    func_0020F9A8(0,view,300,60,text);
    func_0020F518(0,110,110,200,alpha);
    if (D_004C3740.x38==1) func_0020F9A8(0,view,120,80,D_0028E858);
    else func_0020F9A8(0,view,120,80,D_0028E868);
    if (D_004C3740.x38==0 || D_004C3740.x38==2) {
        if (D_004C3740.x30==0x0FFFFFFF) {
            func_0020F9A8(0,view,300,80,D_0028E878);
            return;
        }
        func_0020F518(0,88,152,73,alpha);
        FormatHmsDiscard(D_004C3740.x30, (char *)(text));
        func_0020F9A8(0,view,300,80,text);
        return;
    }
    func_0020F518(0,88,152,73,alpha);
    FormatHmsDiscard(D_004C3740.x18, (char *)(text));
    func_0020F9A8(0,view,300,80,text);
}
