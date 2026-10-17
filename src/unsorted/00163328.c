/* UNIT 0x00163328 0x00163418 */
struct Rider {
    char pad0[0x1E40];
    int x1E40[2];
    char pad1E48[0x7978 - 0x1E48];
    int x7978;
    char pad797C[0x10];
    int x798C;
};

struct Obj {
    struct Rider *x0;
    int x4;
    int x8;
};

struct World {
    char pad0[0x50130];
    struct Rider *x50130[2];
    char pad50138[0x70];
    int x501A8;
};

struct Scores {
    char pad0[0x1C];
    int x1C[2];
};

extern struct World D_002AF2C0;
extern struct Scores D_0038DB60;
extern int D_004B26AC[];

int func_00163328(struct Obj *obj)
{
    struct Rider *rider = obj->x0;
    int *values = rider->x1E40;
    struct Rider *other;
    int index;
    int rank;
    int score;
    int otherScore;

    if (D_004B26AC[0] == 16) {
        index = rider->x7978;
        other = D_002AF2C0.x50130[index ^ 1];
        if (other != rider) {
            score = D_0038DB60.x1C[index];
            otherScore = D_0038DB60.x1C[index ^ 1];
            if (rider->x798C < other->x798C && score >= otherScore) {
                obj->x8 = values[0];
            } else if (other->x798C < rider->x798C && otherScore >= score) {
                obj->x8 = values[1];
            }
        }
    } else {
        rank = rider->x798C;
        if (rank == 1) {
            obj->x8 = values[0];
        } else if (rank == D_002AF2C0.x501A8) {
            obj->x8 = values[1];
        }
    }
    return obj->x8;
}
