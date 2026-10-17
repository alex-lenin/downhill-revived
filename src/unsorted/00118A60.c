/* UNIT 0x00118A60 0x00118BB8 */
struct Node { float pos[4]; char x10[4]; unsigned short flags; unsigned char numLinks; char x17; short links[4]; };
struct Graph { char pad[0x10064]; int visits; };

/* FLT_MAX in .sdata. Taking its address before the early-out checks puts the
   lui in an earlier block, which gives the retail order in the loop preheader. */
extern float D_0029C120[];

/* Shortest path length from node to target over the link graph (nodes are
   0x20 bytes, links are byte offsets / 0x20 from g). */
float func_00118A60(struct Graph *g, struct Node *node, struct Node *target) {
    int i;
    float best, dist;
    float delta[4] __attribute__((aligned(16)));
    int visits;
    float *maxDist;
    struct Node *next;

    if (node == target) return 0.0f;
    maxDist = D_0029C120;
    visits = g->visits++;
    if (visits >= 101 || node->pos[2] < target->pos[2] || ((node->flags >> 5) & 1) || !node->numLinks)
        return 1048576.0f;
    i = 0;
    best = *maxDist;
    while (i < node->numLinks) {
        next = (struct Node *)((char *)g + (node->links[i] << 5));
        __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(delta) : "r"(node), "r"(next));
        __asm__("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(dist) : "f"(delta[0]), "f"(delta[1]), "f"(delta[2]));
        if (next != target) dist += func_00118A60(g, next, target);
        if (dist < best) best = dist;
        i++;
    }
    return best;
}
