/* UNIT 0x002303E8 0x002305F0 */
extern char D_006754E8[];
extern void *memcpy(void *, const void *, unsigned int);
extern void func_00242398(void *,int *,int *,int *,int *);
extern int func_002304C0(char *,int,char *,int,char *,int,char *,int);
extern void func_00242448(void *,int);
typedef struct {int x0,x4;unsigned int x8;int xC;} Obj;
int func_002303E8(void *unused,Obj *obj,char *base) {
 int a,b,c,d; int size,first,rest,result,available,remainder; unsigned int pos; unsigned int length;
 length=*(unsigned int *)(base+0x50008);
 pos=obj->x8+4;
 size=(int)(base+length);
 if(pos>=(unsigned int)size) pos-=length;
 rest=obj->xC-4;
 available=size-pos;
 first=(available>=rest)?rest:available;
 remainder=rest-first;
 func_00242398(D_006754E8,&a,&b,&c,&d);
 result=func_002304C0((char *)a,b,(char *)c,d,(char *)pos,first,base,remainder);
 func_00242448(D_006754E8,result);
 return result>0;
}

/* Copies two source pieces (src1/n1, src2/n2) into two destination pieces (dst1/len1, dst2/len2). */
int func_002304C0(char *dst1, int len1, char *dst2, int len2, char *src1, int n1, char *src2, int n2)
{
    int rest;

    if (len1 + len2 < n1 + n2) {
        return 0;
    }
    if (n1 >= len1) {
        memcpy(dst1, src1, len1);
        memcpy(dst2, src1 + len1, n1 - len1);
        memcpy(dst2 + n1 - len1, src2, n2);
    } else {
        rest = len1 - n1;
        if (n2 >= rest) {
            memcpy(dst1, src1, n1);
            memcpy(dst1 + n1, src2, rest);
            memcpy(dst2, src2 + len1 - n1, n2 - rest);
        } else {
            memcpy(dst1, src1, n1);
            memcpy(dst1 + n1, src2, n2);
        }
    }
    return n1 + n2;
}
