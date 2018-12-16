#!/usr/bin/bash
FILES=files/*s

for file in $FILES; do
   mips-as -a $file -o ./files/$(basename "$file" .s).o.ref
done
