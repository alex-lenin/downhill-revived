/* UNIT 0x001DDE70 0x001DE3E0 .rodata=0x2935C0:0x293660 */
typedef struct Pair {
    int x0;
    int x4;
} Pair;

typedef struct Obj {
    char pad0[0x44];
    Pair x44[2];
    char pad54[0x58 - 0x54];
    int x58;
    char pad5C[0x70 - 0x5C];
    int x70;
    char pad74[0x84 - 0x74];
    int x84;
    int x88;
    int x8C;
    int x90;
    char pad94[0x98 - 0x94];
    int x98;
    int x9C;
    int xA0;
    char padA4[0xB0 - 0xA4];
    float xB0;
    float xB4;
    float xB8;
    float xBC;
    char padC0[0xC4 - 0xC0];
    int xC4;
    char padC8[0x178 - 0xC8];
    int x178;
    char pad17C[0x188 - 0x17C];
    int x188;
    int x18C;
    int x190;
    int x194;
    int x198;
    char pad19C[0x1A0 - 0x19C];
    int sound[10];
    int x1C8[5];
    char pad1DC[0x1F0 - 0x1DC];
    int x1F0;
    int x1F4;
    int x1F8;
    int x1FC;
    int x200;
    char pad204[0x210 - 0x204];
    int x210;
    int x214;
    int x218;
    int x21C;
    int x220;
} Obj;

extern unsigned char D_004B25A0[];
extern unsigned char D_004225A0[];
extern char D_0043DCC0[];

extern int func_001DEDE0(void);
extern void func_001C6EA8(void *);
extern void func_001DE3E0(void *);
extern void func_001DF560(void *, const char *, int *);
extern int func_00222B08(int, int);
extern int func_00238F48(void);
extern char *func_00254544(char *, const char *);
extern char *strncat(char *, const char *, unsigned int);
extern void func_0019BDA0(char *, int, int);

typedef struct NoteList { char names[10][3]; } NoteList;
static const NoteList noteInit = {{"C", "E", "E2", "G", "HH", "O", "P", "T1", "T2", "T"}};

void func_001DDE70(Obj *obj)
{
    int i;
    int tries;
    char path[48];
    NoteList notes;

    if (D_004B25A0[0x350] == 0) {
        if (func_001DEDE0()) {
            if (obj->x218 == 0) {
                obj->xB0 = 0.2f;
                obj->xB4 = 2.0f;
                obj->xB8 = 1.8f;
                obj->xBC = 3.0f;
                obj->x98 = 0;

                for (i = 0; i < 5; i++) {
                    obj->x1C8[i] = 0;
                }

                tries = 0;
                obj->x1FC = func_00238F48();
                obj->x200 = func_00238F48();
                obj->x210 = 0;
                obj->x214 = 0;
                obj->x21C = 0;
                obj->x220 = 0;
                obj->x18C = 0;
                obj->x190 = 0;
                obj->x194 = 0;
                obj->x8C = 0;
                obj->x1F4 = 0;
                obj->xA0 = 0x400;
                obj->x198 = 1;
                obj->x188 = 1;
                obj->x1F0 = func_00238F48();

                {
                    notes = noteInit;

                    obj->x84 = func_00222B08(0, 9);
                    if (obj->x84 == obj->x88) {
                        do {
                            obj->x84 = func_00222B08(0, 9);
                            tries++;
                            if (tries >= 4) {
                                obj->x84 = obj->x88 + 1;
                                if (obj->x84 >= 10) {
                                    obj->x84 = 0;
                                }
                            }
                        } while (obj->x84 == obj->x88);
                    }

                    obj->x88 = obj->x84;

                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "HIGH-L.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[0]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "HIGH-R.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[1]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "MED-L.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[2]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "MED-R.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[3]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "LOW-L.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[4]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "LOW-R.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[5]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "DRUM-L.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[6]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "DRUM-R.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[7]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "LOW-L.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[8]);
                    func_00254544(path, notes.names[obj->x84]);
                    strncat(path, "LOW-R.VAG", 40);
                    func_001DF560(obj, path, &obj->sound[9]);
                }
                obj->x9C = 0x400;
            } else {
                func_001C6EA8(D_004B25A0 + 0x374);
            }
        } else if (obj->x218 == 0) {
            obj->x1C8[obj->x190] = 0;
        } else {
            func_001C6EA8(D_004B25A0 + 0x374);
        }

        func_001DE3E0(obj);

        if (obj->x218 == 0) {
            for (i = 0; i < 2; i++) {
                obj->x44[i].x0 = 1;
            }
        }

        obj->x90 = 0;
        obj->x58 = 0;
        obj->x178 = 0;
        obj->x70 = 0;
        obj->xC4 = 1;

        func_0019BDA0(D_0043DCC0, 0x19, D_004225A0[0x12195]);
        func_0019BDA0(D_0043DCC0, 0x1E, D_004225A0[0x12196]);
        func_0019BDA0(D_0043DCC0, 0x23, D_004225A0[0x12197]);
    }
}
