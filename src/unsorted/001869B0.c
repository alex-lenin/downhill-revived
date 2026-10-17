/* UNIT 0x001869B0 0x00186B88 .rodata=0x28C6B0:0x28C6D0 .sbss=0x29DE24:0x29DE28 */
struct Task { unsigned int x0; char pad4[0xC84]; int xC88,xC8C,xC90; };
struct Context { int x0,x4,x8,xC,x10; char pad14[0x8D]; char xA1[1]; };
extern int func_001866F8(struct Task *,struct Context *,int *);
extern int func_00185FF8(struct Context *,int *,int);
extern int func_00186B88(struct Task *,struct Context *,int *);
extern int func_00186818(struct Task *,void *);
extern void func_00184938(struct Context *,int);
extern int func_00246220(int,int,int);
extern int func_002466A0(int,int *,int *);
extern void func_00254D68(void *,void *,int,void *);
extern char D_003D0F40[],D_0029CD80[],D_0028C648[];
int D_0029DE24 __attribute__((section(".sbss"))) = 0;
int func_001869B0(struct Task *task,struct Context *ctx) {
 int commands[1];
 if(ctx->x0) return 1;
 switch(task->x0) {
 case 0: if(func_001866F8(task,ctx,&task->xC90)) { task->xC90=0; task->x0++; } break;
 case 1: if(task->xC88>0) task->x0=3; else {func_00246220(ctx->x10,ctx->xC,task->xC8C); task->x0++;} break;
 case 2: commands[0]=11; if(func_002466A0(1,commands,&D_0029DE24)) { if(D_0029DE24<0) {func_00184938(ctx,ctx->x4); return 1;} task->xC90=0; task->x0++; } break;
 case 3: func_00254D68(D_003D0F40,D_0029CD80,task->xC8C,D_0028C648); if(func_00186818(task,D_003D0F40)) {task->x0=5; break;} task->x0++;
 case 4: if(func_00185FF8(ctx,&task->xC90,task->xC8C)) {task->xC90=0; task->x0++;} break;
 case 5: func_00254D68(D_003D0F40,D_0029CD80,task->xC8C,ctx->xA1); if(func_00186818(task,D_003D0F40)) return 1; task->x0++;
 case 6: if(func_00186B88(task,ctx,&task->xC90)) {task->xC90=0; task->x0++; return 1;} break;
 }
 return ctx->x0!=0;
}
