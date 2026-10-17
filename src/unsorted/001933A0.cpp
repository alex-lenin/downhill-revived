/* UNIT 0x001933A0 0x00193478 */
#include "model.h"
struct Flags { unsigned long x0; };
class Base { public:
 virtual void a(); virtual void b(); virtual void c(); virtual Flags *get();
};
class State : public Base { public:
 char pad[0x24C-4]; char x24C;
 void func_001933A0();
};


extern int func_00193478(State *);
extern int D_00768050[];
extern int D_0076FA78[][100];
void State::func_001933A0() {
 int index=func_00192BA8(func_002623C0(this)); int result;
 ((char *)get())[4]=0;
 get()->x0 &= 0xFDFFFFFFFFFFFFFFUL;
 x24C=1;
 result=func_00193478(this);
 D_0076FA78[index][D_00768050[index]++]=result;
}
