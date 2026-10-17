#ifndef DHD_TEAM_ROSTER_H
#define DHD_TEAM_ROSTER_H
/* Team membership records read by 001DB9E0. */
typedef struct TeamRosterItem { int id; char pad4[0x2C]; } TeamRosterItem;
typedef struct TeamRosterTeam { char pad0[0x20]; TeamRosterItem items[11]; char pad230[0x10]; } TeamRosterTeam;
typedef struct TeamRoster { char pad0[0x240]; TeamRosterTeam teams[4]; int counts[4]; } TeamRoster;
typedef char TeamRosterItem_size_check[sizeof(TeamRosterItem) == 0x30 ? 1 : -1];
typedef char TeamRosterTeam_size_check[sizeof(TeamRosterTeam) == 0x240 ? 1 : -1];
typedef char TeamRoster_size_check[sizeof(TeamRoster) == 0xB50 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int func_001DB9E0(int id, int found[4]);
#ifdef __cplusplus
}
#endif
#endif
