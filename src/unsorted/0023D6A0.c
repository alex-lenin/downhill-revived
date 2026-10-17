/* UNIT 0x0023D6A0 0x0023D6C8 */
typedef struct Entry {
    char pad0[0x28];
    float x28;
    float x2C;
    char pad30[0x10];
} Entry;
extern Entry D_0075AA70[];
void func_0023D6A0(int index, float *a, float *b) {
    Entry *entry = &D_0075AA70[index];
    *a = entry->x28;
    *b = entry->x2C;
}
