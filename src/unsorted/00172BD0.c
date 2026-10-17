/* UNIT 0x00172BD0 0x00172CB0 .rodata=0x28B0F0:0x28B110 */
typedef struct Model {
    char pad0[0x10];
    char x10[0x30];
    char x40[0x60];
    char xA0[1];
} Model;

typedef struct Elem {
    char pad0[0x8];
    int state;
    Model *model;
    char pad10[0xD0 - 0x10];
    char xD0[1];
} Elem;

extern void func_001732C0(Elem *elem);
extern void func_00172CB0(Elem *elem);
extern void func_00172D90(Elem *elem);
extern void func_00172E68(Elem *elem);
extern void func_00172FF8(Elem *elem);
extern void func_00173148(Elem *elem);
extern void func_00173CB0(Elem *elem);
extern void func_00173188(Elem *elem);
extern void func_00221F50(void *a, void *b);
extern void func_001737F0(Elem *elem);
extern void func_001D18C8(void *a, void *b);

void func_00172BD0(Elem *elem)
{
    func_001732C0(elem);
    switch (elem->state) {
    case 0:
        break;
    case 1:
        func_00172CB0(elem);
        break;
    case 2:
        func_00172D90(elem);
        break;
    case 3:
        func_00172E68(elem);
        break;
    case 4:
        func_00172FF8(elem);
        break;
    case 5:
        func_00173148(elem);
        break;
    case 6:
        func_00173CB0(elem);
        break;
    case 7:
        func_00173188(elem);
        break;
    }
    func_00221F50(elem->model->x40, elem->model->xA0);
    func_001737F0(elem);
    func_001D18C8(elem->xD0, elem->model->x10);
}
