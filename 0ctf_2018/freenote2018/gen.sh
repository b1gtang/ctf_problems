#!/bin/bash
gcc  -D_FORTIFY_SOURCE=2 -O1 -fPIE -pie -Wl,-z,relro,-z,now freenote2018.c -o freenote2018 && strip freenote2018
