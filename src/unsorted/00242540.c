/* UNIT 0x00242540 0x002426F0 */
typedef struct Obj { int state; char pad4[0x2c]; char *x30; int x34,x38,x3c; int x40; char *x44; int x48,x4c,x50,x54; } Obj;
extern int func_00202B08(void);
extern void func_002426F0(char **,int *,char **,int *,Obj *,int);
extern int func_00242798(char *,int,char *,int,char *,int,char *,int);
int func_00242540(Obj *obj) {
    char *out0,*out2;
    int out1,out3;
    int count=0;
    int result;
    int offset,aligned,space,first,second,value,state;
    char *dest;
    switch(obj->state) {
    case 0: return 0;
    case 1:
        out0=obj->x44 + obj->x54%obj->x48;
        out1=obj->x48-obj->x54;
        out2=0; out3=0;
        break;
    case 2:
        value=func_00202B08();
        func_002426F0(&out0,&out1,&out2,&out3,obj,(value&0xffffff)-(int)obj->x44);
        break;
    case 3: return 0;
    }
transfer:
    offset=(obj->x34-obj->x38+obj->x3c)%obj->x3c;
    aligned=(obj->x38/1024)*1024;
    space=obj->x3c-offset;
    dest=obj->x30+offset;
    first=space<aligned?space:aligned;
    second=aligned-first;
    if(out1+out3>=1024 && first+second>=1024)
        count=func_00242798(out0,out1,out2,out3,dest,first,obj->x30,second);
    obj->x38-=count;
    obj->x54+=count;
    obj->x4c=(obj->x4c+count)%obj->x48;
    result=count;
end:
    return result;
}
