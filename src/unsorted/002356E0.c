/* UNIT 0x002356E0 0x00235790 */
extern char D_00299220[]; /* "SKAT989.cpp" */
extern char D_00299248[]; /* "m_pQueueData" */
extern char D_00299258[]; /* "queue < m_numQueues" */

extern void func_00254E58(char *file, int line, char *expr);

typedef struct QueueData {
    char pad0[0x391C];
    int x391C;
    char pad3920[0x8];
} QueueData;

typedef struct Skat {
    char pad0[0x604];
    QueueData *queueData;
    unsigned int numQueues;
} Skat;

void func_002356E0(Skat *skat, unsigned int queue, int value)
{
    if (skat->queueData == 0) {
        func_00254E58(D_00299220, 0xAC, D_00299248);
        if (skat->queueData == 0) {
            return;
        }
    }
    if (queue >= skat->numQueues) {
        func_00254E58(D_00299220, 0xB0, D_00299258);
        if (queue >= skat->numQueues) {
            return;
        }
    }
    skat->queueData[queue].x391C = value;
}
