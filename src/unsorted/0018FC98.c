/* UNIT 0x0018FC98 0x0018FD70 */
#define MIN(a,b) ((a)<(b)?(a):(b))
struct Data { char pad0[8]; unsigned char x8; char pad9[2]; unsigned char xB; struct Data *xC[1]; };
struct State { int x0; unsigned char x4; char pad5[3]; int x8; struct Data *xC; };
extern int D_0029E4A8;
extern int func_00218498(struct Data *,int);
void func_0018FC98(struct State *self,int *value) {
 struct Data *data=self->xC; int frame; 
 if(data) {
  frame=*value+self->x8;
  if(self->x4) {
   struct Data *part=data->xC[data->x8]; unsigned char limit; int upper,clamped;
   limit=part->xB;
   upper=limit-1;
   clamped=MIN(upper,frame);
   frame=0;
   if(clamped > frame-1) frame=clamped;
   func_00218498(part,(frame+D_0029E4A8)%limit);
  } else {
   unsigned char limit; int upper,clamped;
   limit=data->xB;
   upper=limit-1;
   clamped=MIN(upper,frame);
   frame=0;
   if(clamped > frame-1) frame=clamped;
   func_00218498(data,(frame+D_0029E4A8)%limit);
  }
 }
}
