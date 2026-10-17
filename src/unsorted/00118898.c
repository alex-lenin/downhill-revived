/* UNIT 0x00118898 0x00118A60 .rodata=0x288A30:0x288A78 */
typedef struct Node {
 char pad0[0x14];
 union { unsigned short value; struct { unsigned char low,high; } bytes; } flags;
 unsigned char count, pad17; short links[4];
} Node;
typedef struct Graph { Node nodes[2048]; char pad10000[0x64]; int x10064; } Graph;
extern int func_00118640(Graph *,Node *);
extern Node *func_00118650(Graph *,Node *,int,int);
extern float func_00118A60(Graph *,Node *,Node *);
extern void func_00254D08(char *,int,int);
extern float D_0029C11C[4];
Node *func_00118898(Graph *graph,Node *from,Node *target,int allowBlocked,int allowFlagged) {
 Node *best=0,*node; float bestDistance,distance; int i;
 if(from) {
  if(((from->flags.value >> 5)&1)) return from;
  if((unsigned)(from->count-1)>=4) func_00254D08("getNextPoint::fromPoint %d has numPaths %d (numpaths out of bounds)\n",func_00118640(graph,from),from->count);
  if(target) {
   if(target!=from) {
    bestDistance=D_0029C11C[0]; i=0;
    if(from->count) {
     node=&graph->nodes[from->links[0]];
     while(1) {
      if(node==target) { best=node; break; }
      if(allowBlocked || !((node->flags.value >> 12)&1)) {
       graph->x10064=0;
       distance=func_00118A60(graph,node,target);
       if(distance<bestDistance && distance<1048576.0f) {
        bestDistance=distance;
        if(allowFlagged || !(node->flags.bytes.high&1)) best=node;
       }
      }
      ++i;
      if(i>=from->count) break;
      node=&graph->nodes[from->links[i]];
     }
    }
   }
   if(best) return best;
  }
  best=func_00118650(graph,from,allowBlocked,allowFlagged);
 }
 return best;
}
