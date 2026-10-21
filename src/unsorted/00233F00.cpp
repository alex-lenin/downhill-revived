/* UNIT 0x00233F00 0x00234608 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: register allocation only (total/remaining kept as two pseudos -> extra s4->s3 copy,
   chosen-group slot spilled to the stack instead of $s6, inputs in $s7 not $fp, scan temps renamed);
   best diff 194/450 words, same size */
/* Candidate argument: event data word and its id (-1/-1 when unused). */
struct Pair { int value, id; };
/* Action choice: mask of allowed event kinds (-1 = any), x4 marks records, remaining = uses left. */
struct Choice { unsigned int mask; unsigned short x4; char pad6[6]; short remaining; };
struct Input { int index; unsigned int mask; Choice *choice; int count; int ids[32]; int values[32]; };
struct Entry { int index; unsigned int count; int args[1]; };
/* Weighted response group; entries[] holds `count` variable-length Entry records. */
struct Group { unsigned char weight, count; char pad2[2]; int id, bonus; unsigned int xC, x10, mask; Entry entries[1]; };
/* Event definition: `count` groups and `argCount` data ids. */
struct Info { char pad0[8]; unsigned short count, argCount; int *ids; Group **groups; };
struct Table { char pad0[16]; Choice **choices; };
struct Rec { char pad0[5]; unsigned char flags; char pad6[6]; };
struct Event { int kind, index, mask, id, value; };
extern "C" void *func_001FD430(unsigned int);
extern "C" void func_001FD3B8(void *);
inline void *allocArray(unsigned int count, unsigned int size) { return func_001FD430(count * size); }
/* Same object as include/event_queue.h EventQueue (vtable pointer at 0x5FC); only the fields used here. */
class EventQueue {
    char pad0[0x104]; int lastGroupId[32];  /* per kind: id of the group last fired */
    char pad184[0x1D4-0x184]; Rec *recs; Table *table;
    int x1DC; int (*callback)(Event *, int); int x1E4, callbackArg;
    char pad1EC[0x5FC-0x1EC];
public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual unsigned int Random();
    int func_00234670(Choice *, unsigned int, Pair *);
    int func_00233670(int, Input *, int);
    int func_00233F00(int kind, unsigned int mask, int index, Info *info, int *data);
};
/* Pick one eligible group by weighted random choice (bonus weight for the group fired last time),
 * bind each of its entries to a choice, and fire them via func_00233670. Groups whose entries cannot
 * be bound are dropped and the draw repeats. Returns 1 when a group fired. */
int EventQueue::func_00233F00(int kind, unsigned int mask, int index, Info *info, int *data)
{
    Event event;
    unsigned int bits = 0;
    int accepted = 1;
    int total = 0;
    int i, j;
    int scanLeft;
    unsigned int k;
    Group **slot;
    Group **eligible;
    int count;
    int *indices;
    Group **src, **dst;
    Group *group;
    Entry *entry;
    Pair *args;
    Input *inputs;
    int success;
    int pick, sum;
    if (!info->count) return 0;
    count = 0;
    eligible = (Group **)func_001FD430(info->count * 4);
    for (i = 0; i < info->argCount; i++) {
        if (info->ids[i] >= 0) {
            if (callback) {
                event.mask = mask;
                event.kind = kind;
                event.index = index;
                event.id = info->ids[i];
                event.value = data[i];
                accepted = callback(&event, callbackArg);
                if (accepted) bits |= 1 << i;
            } else {
                if (accepted) bits |= 1 << i;
            }
        }
    }
    unsigned short n = info->count;
    if (n) {
        scanLeft = n;
        src = info->groups;
        dst = eligible;
        do {
            *dst = 0;
            group = *src;
            if (((group->x10 ^ bits) | group->xC) == 0xFFFFFFFF && group->count) {
                if (!mask) mask = 0xFFFFFFFF;
                if (group->mask & mask) {
                    *dst = group;
                    total += group->weight;
                    if (lastGroupId[kind] >= 0 && lastGroupId[kind] == group->id) total += group->bonus;
                    count++;
                }
            }
            src++;
            dst++;
        } while (--scanLeft);
    }
    int remaining = total;
    while (count && remaining > 0) {
        pick = Random() % remaining;
        sum = 0;
        for (i = 0; i < info->count; i++) {
            if (eligible[i]) {
                sum += eligible[i]->weight;
                if (lastGroupId[kind] >= 0 && lastGroupId[kind] == eligible[i]->id) sum += eligible[i]->bonus;
            }
            if (pick < sum) {
                inputs = 0;
                entry = eligible[i]->entries;
                if (eligible[i]->count) {
                    inputs = (Input *)allocArray(eligible[i]->count, sizeof(Input));
                    success = 1;
                } else success = 0;
                for (j = 0; j < eligible[i]->count; j++) {
                    Choice *choice = table->choices[entry->index];
                    slot = &eligible[i];
                    if (!(choice->mask & mask)) {
                        success = 0;
                        remaining -= eligible[i]->weight;
                        if (lastGroupId[kind] >= 0 && lastGroupId[kind] == eligible[i]->id) remaining -= eligible[i]->bonus;
                        eligible[i] = 0;
                        count--;
                        break;
                    }
                    if (choice->remaining) {
                        indices = entry->args;
                        args = 0;
                        if (entry->count) args = (Pair *)func_001FD430(entry->count * sizeof(Pair));
                        for (k = 0; k < entry->count; k++) {
                            if (indices[k] < 0) {
                                args[k].value = -1;
                                args[k].id = -1;
                            } else {
                                args[k].value = data[indices[k]];
                                args[k].id = info->ids[indices[k]];
                            }
                        }
                        if ((inputs[j].index = func_00234670(table->choices[entry->index], entry->count, args)) == -1) {
                            if (args) func_001FD3B8(args);
                        } else goto selected;
                    }
                    success = 0;
                    remaining -= (*slot)->weight;
                    if (lastGroupId[kind] >= 0 && lastGroupId[kind] == (*slot)->id) remaining -= (*slot)->bonus;
                    (*slot) = 0;
                    count--;
                    break;
                selected:
                    inputs[j].count = 0;
                    for (k = 0; k < entry->count; k++) {
                        if (args[k].id >= 0) {
                            inputs[j].ids[inputs[j].count] = args[k].id;
                            inputs[j].values[inputs[j].count] = args[k].value;
                            inputs[j].count++;
                        }
                    }
                    if (args) func_001FD3B8(args);
                    inputs[j].mask = table->choices[entry->index]->mask;
                    if (inputs[j].mask == 0xFFFFFFFF) inputs[j].mask = 0;
                    inputs[j].choice = table->choices[entry->index];
                    entry = (Entry *)(indices + entry->count);
                }
                if (success) {
                    for (j = 0; j < eligible[i]->count; j++) {
                        func_00233670(kind, &inputs[j], index);
                        if (inputs[j].choice->x4) {
                            recs[inputs[j].index].flags |= 1;
                            if (inputs[j].choice->remaining > 0) inputs[j].choice->remaining--;
                        }
                    }
                    lastGroupId[kind] = eligible[i]->id;
                    if (inputs) func_001FD3B8(inputs);
                    if (eligible) func_001FD3B8(eligible);
                    return 1;
                }
                if (inputs) func_001FD3B8(inputs);
                break;
            }
        }
    }
    if (eligible) func_001FD3B8(eligible);
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_00233F00);
#endif
