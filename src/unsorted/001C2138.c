/* UNIT 0x001C2138 0x001C2280 */
extern long D_0029D590[];
extern char D_0028FB60[],D_0028FB78[],D_0028FB90[];
extern char *func_002542C4(char*,const char*);
extern int func_00254D08(const char*,const char*);
extern int func_0025DA60(char*,int);
extern int func_0025DE68(int,int,int);
extern int func_0025E0A0(int,void*,int);
extern void func_0025DCE8(int);
extern void *func_001FD430(int);
extern void func_001FD3B8(void*);
void *func_001C2138(const char *name,int unused,int size,int offset) {
 char buf[1024];
 int fd;
 void *mem;
 *(long*)buf=D_0029D590[0];
 func_002542C4(buf,name);
 fd=func_0025DA60(buf,1);
 if(fd<0) { func_00254D08(D_0028FB60,name); return 0; }
 if(func_0025DE68(fd,offset,0)!=offset) goto seek_error;
 if(!size) {
 size=func_0025DE68(fd,0,2);
 if(size<0) goto seek_error;
 size-=offset;
 if(size<0) size=0;
 if(func_0025DE68(fd,offset,0)==0) goto read_file;
 seek_error:
 func_00254D08(D_0028FB78,name);
 return 0;
 }
 read_file:
 mem=func_001FD430(size);
 if(func_0025E0A0(fd,mem,size)!=size) {
 func_00254D08(D_0028FB90,name);
 if(mem) func_001FD3B8(mem);
 return 0;
 }
 func_0025DCE8(fd);
 return mem;
}
