#!/bin/bash

installpath="/usr/local/include"
libpath="/usr/local/lib"

sudo rm -rf $installpath/bbt/ecs
sudo rm -rf $libpath/libbbt_ecs.so

echo "删除完毕"