#!/bin/bash

shopt -s extglob

rm -f *.fixed.lib

for libfile in *.lib; do
    mkdir -p $libfile.objs1
    mkdir -p $libfile.objs2
    (cd $libfile.objs1; ar x ../$libfile)
    for objfile in $libfile.objs1/*.o; do
        ./BREWRuntimePatcher $objfile $libfile.objs2/${objfile#$libfile.objs1/}
    done
    (cd $libfile.objs2; ar rcs ../${libfile%.lib}.fixed.lib *.o)

done

rm -r *.lib.objs1
rm -r *.lib.objs2

arm-none-eabi-objcopy --redefine-sym setjmp=breSetjmp --redefine-sym longjmp=breLongjmp --globalize-symbol=IThread_SetStack aee.fixed.lib aee.fixed2.lib
# rm *.o
# rm *.fixed.lib
# find -name \*.lib -exec arm-none-eabi-ld -relocatable --whole-archive --allow-multiple-definition {} -o {}.o \;
# find -name \*.lib.o -exec ./BREWRuntimePatcher.exe {} {}.fixed.o \;
# for n in *.lib.o.fixed.o; do rm "${n%.lib.o.fixed.o}.fixed.lib"; ar rcs "${n%.lib.o.fixed.o}.fixed.lib" $n; done
# rm *.o

