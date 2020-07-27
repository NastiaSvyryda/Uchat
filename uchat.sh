#!/bin/zsh
for (( a = 1; a < 1000; a++ ))
do
./uchat 0.0.0.0 30
echo a
done
