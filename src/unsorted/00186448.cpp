/* UNIT 0x00186448 0x00186610 .rodata=0x28C690:0x28C6B0 .sbss=0x29DE1C:0x29DE24 */
typedef struct Job { int state; int x4; int x8; unsigned char xc; } Job;
typedef struct Owner { int x0,x4,x8,xc,x10; void func_00184938(int); } Owner;
int D_0029DE1C __attribute__((section(".sbss"))) = 0;
int D_0029DE20 __attribute__((section(".sbss"))) = 0;
extern char D_003CD958[];
extern char D_0028C658[];
extern void func_00184938(Owner *,int);
extern int func_00185FF8(Owner *,Job *,int);
extern int func_002460F8(int,int,char *,int);
extern int func_00246258(int);
extern int func_002464B8(int,void *,int);
extern int func_002466A0(int,int *,int *);
extern void func_00254544(char *,int);
extern "C" char *strncat(char *,const char *,unsigned);
static __inline__ void advance_job(Job *job) { job->state=job->state+1; }
static __inline__ int fail_job(Owner *owner) { owner->func_00184938(owner->x4); return 1; }
static __inline__ void command_init(int *p,int value) { *p=value; }
int func_00186448(Job *job,Owner *owner) {
    char path[0x50];
    int command;
    int result=0;
    if(owner->x0) return 1;
    if(job->xc) return func_00185FF8(owner,job,job->x8)!=0;
    switch(job->state) {
    case 0:
        func_00254544(path,job->x8);
        strncat(path,D_0028C658,0x4a);
        func_002460F8(owner->x10,owner->xc,path,3);
        advance_job(job);
    case 1:
        command_init(&command,2);
        if(!func_002466A0(1,&command,&D_0029DE1C)) goto done;
        if(D_0029DE1C==-1) goto error;
        goto advance;
    case 2:
        D_003CD958[0x104]=0;
        D_003CD958[0x144]=0;
        D_003CD958[0x184]=0;
        func_002464B8(D_0029DE1C,D_003CD958,0x3c4);
        advance_job(job);
    case 3:
        command_init(&command,6);
        if(!func_002466A0(1,&command,&D_0029DE20)) goto done;
        if(D_0029DE20>=0) goto advance;
error:
        return fail_job(owner);
advance:
        advance_job(job);
        goto done;
    case 4:
        func_00246258(D_0029DE1C);
        advance_job(job);
    case 5:
        command_init(&command,3);
        if(func_002466A0(1,&command,&D_0029DE20)) {
            result=1;
            if(D_0029DE20<0) {
                func_00184938(owner,owner->x4);
                return 1;
            }
            return result;
        }
        break;
    }
done:
    return result;
}
