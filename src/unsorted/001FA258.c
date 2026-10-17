/* UNIT 0x001FA258 0x001FA300 */
typedef struct { char pad[0x694]; } Obj;
extern Obj D_004225A0[];
extern struct { int x0; int pad[3]; } D_004B26AC;
extern float D_00275DE8[];
extern int func_00194E30(void *,int);
int func_001FA258(int unused,int kind,int index) {
 int rank,amount,offset,value;
 offset=index*0x694;
 value=D_004B26AC.x0;
 rank=func_00194E30(&D_004225A0[index],value);
 if(kind==1) amount=200;
 else if(kind==2) amount=100;
 else if(kind==3) amount=50;
 else return 0;
 return (int)((float)amount*D_00275DE8[rank]);
}
