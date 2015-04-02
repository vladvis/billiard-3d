#!/bin/bash

rm FIFO
mkfifo FIFO
./Teormech $1 > FIFO
