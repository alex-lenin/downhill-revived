#!/bin/bash
# Usage: sweep.sh <src> — compiles with ee-gcc2.96 under several flag sets, prints asm per set.
W=/opt/eegcc; S=${DHD_COMPILERS:-$(cd "$(dirname "$0")/../compilers" && pwd)}; C=$W/ee-gcc2.96
B=$(dirname $(find $C -name cc1 -type f | head -1))/
mkdir -p $W/work; cd $W/work
for lang in c c++; do
 ext=c; [ $lang = c++ ] && ext=cc
 cp "$S/../match/$1" in.$ext
 while read -r flags; do
  echo "######## [$lang] $flags"
  $C/bin/ee-gcc -B $B $flags -S in.$ext -o - 2>&1 | grep -vE '^\s*\.|^$|gcc2_compiled|__gnu_compiled' | tr '\n' ';' | sed 's/\t/ /g'; echo
 done <<FL
-O1 -G0
-O2 -G0 -fno-optimize-sibling-calls
-O2 -G0 -fno-optimize-sibling-calls -fno-schedule-insns
-O3 -G0 -fno-optimize-sibling-calls
-O1 -G0 -fschedule-insns
FL
done
