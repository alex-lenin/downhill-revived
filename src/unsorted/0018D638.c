/* UNIT 0x0018D638 0x0018D688 */
typedef struct Item { char pad[20]; } Item;
extern Item D_003F4BA0[20];
extern void func_0018D6F0(Item *);
void func_0018D638(void) {
 Item *p=D_003F4BA0;
 do { func_0018D6F0(p); p++; } while((int)p<(int)(D_003F4BA0+20));
}
