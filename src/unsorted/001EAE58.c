/* UNIT 0x001EAE58 0x001EAEF8 */
typedef struct Pos { char pad0[0x18]; float x18; } Pos;
typedef struct Rider { char pad0[0x792C]; Pos *x792C; } Rider;
typedef struct World { char pad0[0x50130]; Rider *riders[1]; } World;

extern World D_002AF2C0[];
extern int D_004B2840[];

int func_001EAE58(void) {
 int found = 0;
 float limit = -10000.0f;
 World *world;
 Rider **p;
 Rider *rider;
 if (D_004B2840[0] == 3) {
  limit = -12225.0f;
 } else if (D_004B2840[0] == 20) {
  limit = -6460.0f;
 }
 world = D_002AF2C0;
 rider = world->riders[0];
 if (rider != 0) {
  p = world->riders;
  do {
   if (rider->x792C->x18 < limit) {
    found = 1;
   }
   rider = *++p;
  } while (rider != 0);
 }
 return found;
}
