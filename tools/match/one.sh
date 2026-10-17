#!/bin/bash
# Usage: one.sh <src> <flags...> — compile C++ with ee-gcc2.96, print code only.
W=/opt/eegcc; S=${DHD_COMPILERS:-$(cd "$(dirname "$0")/../compilers" && pwd)}; C=$W/ee-gcc2.96
B=$(dirname $(find $C -name cc1 -type f | head -1))/
mkdir -p $W/work; cd $W/work; cp "$S/../match/$1" in.cc; shift
$C/bin/ee-gcc -B $B "$@" -S in.cc -o - 2>&1 | grep -vE '^\s*\.|^$|gcc2_compiled|__gnu_compiled|^\$L|^__FRAME|^\$LS|^\$LE'
