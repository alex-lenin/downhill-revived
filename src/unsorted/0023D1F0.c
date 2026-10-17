/* UNIT 0x0023D1F0 0x0023D2B0 */
typedef struct Item { int x0; char pad[12]; float x10,x14,x18; } Item;
typedef struct Entry { int x0; Item *x4,*x8,*xC; char pad[0xe0]; } Entry;
extern Entry D_00528360[];
extern float D_0075A9C0[];
void func_0023D1F0(float *pos,int index) {
 Entry *entry=&D_00528360[index];
 if(entry->x4) { entry->x4->x10=pos[0]; entry->x4->x14=pos[1]; entry->x4->x18=pos[2]+D_0075A9C0[index]; }
 if(entry->x8) { entry->x8->x10=pos[0]; entry->x8->x14=pos[1]; entry->x8->x18=pos[2]; }
 if(entry->xC) { entry->xC->x10=pos[0]; entry->xC->x14=pos[1]; entry->xC->x18=pos[2]; }
}
