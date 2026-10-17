/* UNIT 0x00242C80 0x00242D08 */
typedef struct Node { struct Node *x0,*x4; int x8; unsigned short xC,xE; float x10[4]; } Node;
extern float func_00221C80(float *, void *);
unsigned short func_00242C80(Node *node, void *point) {
 unsigned short result=512; int side;
 while(node) {
 side=func_00221C80(node->x10,point)<0.0f;
 if(side) result=node->xE; else result=node->xC;
 if(side) node=node->x4; else node=node->x0;
 }
 return result;
}
