#!/bin/bash

testpath="test"
execname="sengfmt2.py"

if [[ $1 != "" ]]; then
    python3 $execname $testpath/in$1.txt > out.txt

    result=$(diff out.txt test/out$1.txt)

    if [ "$result" == "" ]
    then
        echo "Test $f: PASS"
    else
        echo "Test $f: FAIL"
        echo "Diff output:"
        echo "$result"
    fi
    exit
fi

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
    echo -n "Run test $f? [Y/n] "
    read answer

    if ! [ "$answer" == "" ] && ! [ "$answer" != "${answer#[Yy]}" ] ;then
        echo Skipping
        continue
    fi

    python3 $execname $testpath/in$f.txt > out.txt

    result=$(diff out.txt test/out$f.txt)

    if [ "$result" == "" ]
    then
        echo "Test $f: PASS"
    else
        echo "Test $f: FAIL"
        echo "Diff output:"
        echo "$result"
    fi
done

echo "Testing finished, deleting output file"
rm out.txt
