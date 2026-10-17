/* UNIT 0x001DC328 0x001DC4B8 */
typedef struct Score { char pad[0x18]; float x18; } Score;
typedef struct Entry { char p0[0x30]; Score *x30, *x34; char p38[0x28]; } Entry;
typedef struct State { Entry entries[100]; int x2580[5]; int x2594[4]; int x25A4[4]; char p25B4[0x10]; int x25C4; } State;
typedef struct Global { char pad[0x288]; int x288; } Global;
typedef struct Values { char pad[0x50130]; int values[4]; } Values;
extern Values D_002AF2C0;
extern Global D_004B25A0;
extern int func_00222BD8(int, int);
extern int func_001DC4B8(State *, int, int), func_001DC910(Entry *, int);
void func_001DC328(State *s) {
    Global *config = &D_004B25A0;
    Values *global = &D_002AF2C0;
    int *values = global->values;
    int i = 0, sorted, j, k;
    int *dest = s->x25A4;
    do {
        int value = *values;
        if (value) {
            int flag = (s->x25C4 == 0);
            *(int *)((char *)dest - 0x24) = value;
            *(int *)((char *)dest - 0x10) = flag;
        }
        values++;
        *dest = i + func_00222BD8(0, config->x288);
        i++; dest++;
    } while (i < 4);
    /* Bubble-sort entries by score key (x30 preferred, else x34, else 0.0). */
    sorted = 1;
    while (1) {
        for (j = 0; j < s->x25C4 - 1; j++) {
            float first, second;
            Entry *next;
            Entry *current = &s->entries[j];
            Score *p = current->x30;
            Score *q;
            if (p || (p = current->x34, first = 0.0f, p)) first = p->x18;
            next = (Entry *)((char *)s + j * 0x60 + 0x60);
            q = next->x30;
            second = q ? q->x18 : (next->x34 ? next->x34->x18 : 0.0f);
            if (first < second) { sorted = 0; func_001DC4B8(s, j, j + 1); }
        }
        if (sorted) break;
        sorted = 1;
    }
    for (k = 0; k < s->x25C4; k++) { func_001DC910(&s->entries[k], k); }
}
