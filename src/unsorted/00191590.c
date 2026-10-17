/* UNIT 0x00191590 0x00191640 */
typedef struct Panel {
    char pad0[0xA0];
} Panel;

typedef struct Hud {
    int x0;
    char pad4[0xC];
    Panel panels[3];
    int x1F0[3];
    float x1FC;
    float x200;
    int x204;
    int x208[3];
    int x214;
    int x218;
    int x21C;
    int x220;
    int x224;
    int x228;
    int x22C;
} Hud;

extern void func_00191978(Panel *panel);

void func_00191590(Hud *hud)
{
    int i;

    hud->x0 = 0;
    for (i = 0; i < 3; i++) {
        func_00191978(&hud->panels[i]);
        hud->x1F0[i] = 0;
        hud->x208[i] = 0;
    }
    hud->x1FC = 1.4666667f;
    hud->x200 = 2.5f;
    hud->x21C = 2;
    hud->x204 = 0;
    hud->x220 = 0;
    hud->x224 = 0;
    hud->x22C = 0;
    hud->x214 = 0;
    hud->x218 = 0;
    hud->x228 = 1;
}
