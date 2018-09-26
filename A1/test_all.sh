#!/bin/sh

files="
00
01
02
03
04
05
06
07
08
09
10
"

for f in $files 
do
    ./sengfmt test/in$f.txt > out.txt
    echo "Test $f: $(diff out.txt test/out$f.txt)"
done
