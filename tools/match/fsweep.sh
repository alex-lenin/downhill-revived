#!/bin/bash
# Grade flag sets on func_10A3B0 (c4 form) vs target instruction order.
W=/opt/eegcc; S=${DHD_COMPILERS:-$(cd "$(dirname "$0")/../compilers" && pwd)}; C=$W/ee-gcc2.96
B=$(dirname $(find $C -name cc1 -type f | head -1))/
cd $W/work
cat > f2.c <<'X'
struct S { int x0, x4, x8; unsigned short xc, xe; int x10; };
extern void func_2038E0(void *, int);
extern void func_203590(void *, int, int, int);
void f(struct S *s) { int buf[4]; buf[2] = 0; func_203590(buf, s->x8, s->xc & 0x7FF, s->xe); func_2038E0(buf, 1); }
X
T="subu \$sp,\$sp,32;move \$2,\$4;sd \$31,16(\$sp);move \$4,\$sp;sw \$0,8(\$sp);lhu \$6,12(\$2);lw \$5,8(\$2);lhu \$7,14(\$2);jal func_203590;andi \$6,\$6,0x7ff;move \$4,\$sp;jal func_2038E0;li \$5,1;ld \$31,16(\$sp);j \$31;addu \$sp,\$sp,32;"
base="-G0 -fno-optimize-sibling-calls"
for o in -O1 -O2 -O3 -Os; do for x in "" -fno-schedule-insns -fschedule-insns -fno-schedule-insns2 -fno-gcse -fno-regmove -fno-expensive-optimizations -fno-caller-saves -fno-strength-reduce -fno-cse-follow-jumps -fno-rerun-cse-after-loop -fno-peephole -fno-force-mem -fforce-mem -fno-thread-jumps -fno-defer-pop -fomit-frame-pointer -fno-omit-frame-pointer -fno-common "-fno-schedule-insns -fno-regmove" "-fno-schedule-insns -fno-gcse"; do
  for lang in c c++; do
   [ $lang = c ] && fn=f2.c || { cp f2.c f2.cc; fn=f2.cc; }
   got=$($C/bin/ee-gcc -B $B $o $base $x -x $lang -S $fn -o - 2>/dev/null | grep -vE '^\s*\.|^$|:$|#nop|^\$L' | sed -E 's/\s+#.*//; s/^\s+//; s/\s+/ /' | sed -E 's/__F[A-Za-z0-9]*//' | tr '\n' ';')
   [ -n "$DBG" ] && echo "$got"
   [ "$got" = "$T" ] && echo "MATCH [$lang] $o $base $x"
  done
done; done
echo done
