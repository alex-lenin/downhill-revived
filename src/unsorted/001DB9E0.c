/* UNIT 0x001DB9E0 0x001DBAB0 */
#include "team_roster.h"
typedef TeamRoster Teams;

typedef struct Global {
    char pad000[0x288];
    int count;
} Global;

extern Teams D_00398C40;
extern Global D_004B25A0;

/* Sets found[t] for each team t that has an item with this id; returns whether any did. */
int func_001DB9E0(int id, int *found) {
    int result;
    int k;
    int i;
    int j;
    Teams *teams;

    result = 0;
    for (k = 3; k >= 0; k--) {
        found[k] = 0;
    }
    i = 0;
    if (D_004B25A0.count > 0) {
        teams = &D_00398C40;
        do {
            j = 0;
            if (teams->counts[i] > 0) {
                do {
                    if (teams->teams[i].items[j].id == id) {
                        found[i] = 1;
                        result = 1;
                        break;
                    }
                    j++;
                } while (j < D_00398C40.counts[i]);
            }
            i++;
        } while (i < D_004B25A0.count);
    }
    return result;
}
