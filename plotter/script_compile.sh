#!/bin/bash                                                                                                                                                                                                 

echo ""
echo "------------------------------------------------"

path_actual=${PWD}
echo "actual  path --> " $path_actual

cd $path_package
echo "package path --> " $path_package

echo -e "------------------------------------------------ \n"

cd build
path_build_directory=${path_package}"/build"
echo "build  path --> " $path_build_directory

echo -e "------------------------------------------------ \n"

echo "comando --> cmake ../source"

echo "------------------------------------------------"

cmake ../source

echo -e "------------------------------------------------ \n"

echo "comando --> make"

echo "------------------------------------------------"

make -j 6

echo -e "------------------------------------------------ \n"

cd ..

cd $path_actual
echo "come back to actual path --> "
pwd

echo "------------------------------------------------"

echo "end script"

echo -e "------------------------------------------------ \n"
