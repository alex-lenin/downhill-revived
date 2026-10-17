#!/bin/bash
# Usage: try.sh <src.c> [extra flags]. Runs inside WSL; 32-bit ee-gcc needs a native Linux FS (EOVERFLOW on /mnt).
W=/opt/eegcc; S=${DHD_COMPILERS:-$(cd "$(dirname "$0")/../compilers" && pwd)}
mkdir -p $W/work
for c in ${CC_LIST:-ee-gcc2.96}; do
  [ -d $W/$c ] || cp -r $S/$c $W/
done
src=${1:-t.c}; shift; cp "$S/../match/$src" $W/work/in.c; cd $W/work
for c in ${CC_LIST:-ee-gcc2.96}; do
  echo "== $c"
  B=$(dirname $(find $W/$c -name cc1 -type f | head -1))/
  $W/$c/bin/ee-gcc -B $B -O2 -G0 "$@" -S in.c -o - 2>&1 | grep -vE '^\s*\.|^$|gcc2_compiled' | head -80
done
