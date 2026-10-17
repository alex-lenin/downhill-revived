/* UNIT 0x00233670 0x002337C0 */
struct Rec { int x0; unsigned char x4; char pad[3]; int x8; };
struct Input { int index, x4, x8, count; int a[32]; int b[32]; };
struct Event { int x0,x4,x8,xC,x10,x14,x18,x1C,count; int a[32]; int b[32]; char pad[12]; };
extern "C" void func_00234EC8(void *, int, int *, int *);
class Obj {
 char pad0[0x1D0]; int count; Rec *recs; int x1D8; void (*callback)(Event *,int); int x1E0; int x1E4; char pad1E8[0x5FC-0x1E8];
public:
 virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual int Value();
 int func_00233670(int arg1, Input *input, int arg3);
};
int Obj::func_00233670(int arg1, Input *input, int arg3) {
 Event event; int value; int index; Rec *rec; int n; int *src; int *dst;
 value=Value();
 if (!recs) return 1;
 if (!callback) return 1;
 if (input->index >= count) return 1;
 index=input->index; rec=&recs[index];
 event.x0=arg1; event.x4=arg3; event.x8=input->x4; event.xC=index; event.x10=rec->x4;
 event.x14=rec->x0; event.x18=recs[index+1].x0-event.x14;
 func_00234EC8(this,index,&event.x14,&event.x1C);
 recs[input->index].x8=value;
 event.count=input->count;
 if (event.count>0) { src=input->b; dst=event.b; n=event.count; do { dst[-32]=src[-32]; *dst=*src; src++; dst++; } while (--n); }
 callback(&event,x1E4); return 0;
}
