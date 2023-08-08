#/bin/bash

sudo cp ../server/thirdparty/libs/libinih.so.0 /usr/local/lib/
cd /usr/local/lib
sudo ln -s libinih.so.0 libinih.so