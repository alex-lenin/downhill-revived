/* UNIT 0x001273B0 0x00127440 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct Matrix {
    u128 row[4];
};

struct Transform {
    char pad[0x40];
    Matrix x40;
};

class Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Transform *GetTransform();
};

struct Obj {
    char pad[0x130];
    Part *x130;
};

void func_001273B0(Obj *obj, Matrix *matrix)
{
    if (obj->x130 != 0 && obj->x130->GetTransform() != 0) {
        Transform *t = obj->x130->GetTransform();

        /* 64-byte matrix copy (as sceVu0CopyMatrix) */
        __asm__ volatile (
            "lq $8, 0x0(%4)
	"
            "lq $9, 0x10(%4)
	"
            "lq $10, 0x20(%4)
	"
            "lq $11, 0x30(%4)
	"
            "sq $8, %0
	"
            "sq $9, %1
	"
            "sq $10, %2
	"
            "sq $11, %3"
            : "=m"(t->x40.row[0]), "=m"(t->x40.row[1]), "=m"(t->x40.row[2]), "=m"(t->x40.row[3])
            : "r"(matrix)
            : "$8", "$9", "$10", "$11");
    }
}
