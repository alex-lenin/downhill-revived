/* UNIT 0x00186E10 0x00187008 .rodata=0x28C6D0:0x28C6E0 */
typedef struct Obj { char pad[0xC8C]; char *xC8C; } Obj;
typedef struct Input { int x0,x4,x8,xC,x10; char pad14[0x8D]; char xA1[1]; } Input;
extern char *func_00254544(char*,const char*);
extern char *strncat(char*,const char*,unsigned int);
extern unsigned int strlen(const char*);
extern int func_0020D930(char*,char*);
extern int func_002460F8(int,int,char*,int);
extern int func_002466A0(int,int*,int*);
extern int func_002464B8(int,char*,int);
extern int func_00246258(int);
extern void func_00184938(Input*,int);
extern char D_0029CD90[];
typedef struct Prefix { char text[9]; } Prefix;
const Prefix D_0028C6D0={{92,'M','E','M','C','R','D',92,0}};
void func_00186E10(Obj *obj,Input *in) {
    Prefix prefix=D_0028C6D0;
    char buffer[0x19000];
    char path[0x50];
    int cmd,fd,result,size,len;
    func_00254544(path,prefix.text);
    strncat(path,in->xA1,0x4A);
    strncat(path,D_0029CD90,2);
    size=func_0020D930(buffer,path);
    len=strlen(obj->xC8C);
    func_00254544(path,obj->xC8C);
    path[len]='/';
    func_00254544(path+len+1,in->xA1);
    func_002460F8(in->x10,in->xC,path,0x203);
    cmd=2;
    func_002466A0(0,&cmd,&fd);
    if(fd<0) { func_00184938(in,in->x4); }
    else {
        func_002464B8(fd,buffer,size);
        cmd=6;
        func_002466A0(0,&cmd,&result);
        if(result<0) { func_00184938(in,in->x4); }
        else {
            func_00246258(fd);
            cmd=3;
            func_002466A0(0,&cmd,&result);
            if(result<0) func_00184938(in,in->x4);
        }
    }
}
