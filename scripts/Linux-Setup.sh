#!/bin/bash
cd ..

chmod +x vendor/bin/premake/linux/premake5
./vendor/bin/premake/linux/premake5 gmake2

cd -
