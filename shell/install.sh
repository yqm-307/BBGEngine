#!/bin/bash

installpath="/usr/local/include"
libpath="/usr/local/lib"

cd ..
shell/bbt_copy_header_dir.sh $installpath bbt ecs

sudo cp build/lib/libbbt_ecs.so /usr/local/lib/