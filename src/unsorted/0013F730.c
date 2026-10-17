/* UNIT 0x0013F730 0x0013F940 */
typedef struct { char pad0[0x500]; int x500[64],x600[64]; signed char x700[64]; char pad740[0x80]; unsigned char x7C0[64]; } Menu;
typedef struct { char pad0[0x12190]; unsigned char unlock[0x78]; int money; } Save;
typedef struct { char key[4]; int x4,x8; } Item;
typedef struct { char pad0[0x500]; unsigned char x500,x501; } Message;
extern Save D_004225A0;
extern Item D_00289AB8,D_00289AC8,D_00289AD8,D_00289AE8;
extern Message D_00363390;
extern char D_003F52D0[],D_004C8508[];
extern int D_004B25D8[];
extern int func_0013F6F8(Menu *,int);
extern void func_00194290(Save *,int),func_001F9C78(void *,Item *),func_0013A480(void),func_00190660(void *,Message *);
void func_0013F730(Menu *menu,int slot) {
 Item item; int index;
 index=func_0013F6F8(menu,menu->x600[slot]);
 if(index>=0) {
 D_004225A0.unlock[index]=1;
 D_004225A0.money=(D_004225A0.money/20-menu->x500[slot])*20;
 if(index==21) { func_00194290(&D_004225A0,D_004B25D8[0]); }
 else if(index==17) { item=D_00289AB8; func_001F9C78(D_004C8508,&item); func_0013A480(); }
 else if(index==18) { item=D_00289AC8; func_001F9C78(D_004C8508,&item); func_0013A480(); }
 else if(index==19) { item=D_00289AD8; func_001F9C78(D_004C8508,&item); func_0013A480(); }
 else if(index==20) { item=D_00289AE8; func_001F9C78(D_004C8508,&item); func_0013A480(); }
 menu->x500[slot]=0;
 if(menu->x700[slot]>=0) menu->x700[slot]=1;
 D_00363390.x500=0;
 D_00363390.x501=menu->x7C0[slot];
 func_00190660(D_003F52D0,&D_00363390);
 }
}
