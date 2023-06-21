#!/bin/bash
cp /home/wenteng/credu_try/try1.c $(pwd)
cp /home/wenteng/credu_try/try2.h $(pwd)
gcc try1.c try2.c
./a.out | grep "hello world"
