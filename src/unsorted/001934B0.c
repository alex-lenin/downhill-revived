/* UNIT 0x001934B0 0x00193630 */
typedef struct Entry { int x0; unsigned int **x4; char pad8[0x248]; } Entry;
extern Entry D_003F5690[];
extern int D_00768050[17], D_0076F9E8[17], D_0076FA78[17][100], D_00771508[17][100];

static __inline__ unsigned int get_id(Entry *entry) { return **entry->x4 >> 18; }

void func_001934B0(void) {
 int group, i, j;
 for (group = 0; group < 17; group++) {
  D_0076F9E8[group] = 0;
  for (i = 0; i < D_00768050[group]; i++) {
   int unique = 1;
   int index = D_0076FA78[group][i];
   j = 0;
   while (j < i) {
    if (get_id(&D_003F5690[index]) == D_00771508[group][j]) { unique = 0; break; }
    ++j;
   }
   if (unique) {
    int n = D_0076F9E8[group]++;
    D_00771508[group][n] = get_id(&D_003F5690[index]);
   }
  }
 }
}
