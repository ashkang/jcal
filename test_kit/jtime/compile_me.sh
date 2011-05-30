#!/bin/bash

CC="gcc"
OPTS="-g"
LIBS="-lreadline -ljalali"

for i in *.c; do
    obj=`echo $i | sed -e 's/\.c//g'`;
    echo -ne "Compiling $i -> $obj ";
    ${CC} ${OPTS} $i ${LIBS} -o $obj

    if [ $? -eq 0 ] ; then
	echo "[OK]";
    else
	echo "[Failed]";
    fi
done