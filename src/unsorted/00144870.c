/* UNIT 0x00144870 0x001449F8 .rodata=0x289C40:0x289C70 */
extern char D_003705C0[];
extern int func_00163B70(void *);
extern int func_00163BE0(void *,int);
extern int func_00163D40(void *,int);
extern void func_00144C78(void *,void *,int,int,int);
void func_00144870(char *base) {
    int count,handle=0,i;
    count=func_00163B70(D_003705C0);
    if(*(int *)(base+0x1554)!=0 && count>0) {
        int *slots;
        for(i=0;i<count;i++) {
            int order[12]={0,4,8,1,5,9,2,6,10,3,7,11};
            int index;
            char *obj;
            int *slot;
            int value;
            slots=(int *)(base+0x1510);
            handle=func_00163BE0(D_003705C0,handle);
            index=order[i];
            obj=base+(index*0x1c0+0x10);
            slot=slots+index;
            value=func_00163D40(D_003705C0,handle);
            func_00144C78(obj,base,*slot,i,value);
            *(int *)(base+order[i]*0x1c0+0x1bc)=1;

        }
    }
}
