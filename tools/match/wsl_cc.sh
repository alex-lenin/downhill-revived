#!/bin/bash
# Usage (inside WSL): wsl_cc.sh CC LANG SRC OUT FLAGS... — compile SRC to OUT with a Linux ee-gcc.
# 32-bit compilers fail with EOVERFLOW on /mnt/*, so the compiler is copied to /opt/eegcc
# and the build happens in a scratch dir there.
set -e
cc=$1 lang=$2 src=$3 out=$4; shift 4
S=$(cd "$(dirname "$0")/../compilers" && pwd)
W=/opt/eegcc; mkdir -p $W/work; [ -d $W/$cc ] || cp -r "$S/$cc" $W/
B=$(dirname $(find $W/$cc -name cc1 -type f | head -1))/
A=$(dirname $(find $W/$cc -path '*/ee/bin/as' -type f | head -1))/
T=$(mktemp -d $W/work/g.XXXXXX); trap 'rm -rf $T' EXIT
cp "$src" $T/in; cd $T
mode=-c; for f in "$@"; do [ "$f" = -S ] && mode=; done
$W/$cc/bin/ee-gcc -B $B -B $A -x $lang $mode in -o out.o "$@"
cp out.o "$out"
