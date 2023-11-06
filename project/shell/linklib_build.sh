#/bin/bash

if [ ! -e "/usr/local/lib/libinih.so.0" ];
then
    sudo cp ../server/thirdparty/libs/libinih.so.0 /usr/local/lib/
    cd /usr/local/lib
    sudo ln -s libinih.so.0 libinih.so
fi

if [ ! -e "/usr/local/lib/liblua.a" ];
then
    sudo cp ../server/thirdparty/libs/liblua.a /usr/local/lib/
fi