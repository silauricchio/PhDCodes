#!/bin/bash  

path_package=${PWD}
path_executable_Plotter=${path_package}"/build/Executable_Plotter"
path_executable_basicPlotter=${path_package}"/build/Executable_basicPlotter"
path_compile="source "$path_package"/script_compile.sh"

echo ""
echo "------------------------------------------------"

echo "created alias:"
echo "run_Plotter                --> path executable        --> "$path_executable_Plotter
echo "run_basicPlotter           --> path executable        --> "$path_executable_basicPlotter

echo "compile                    --> path script compile    --> "$path_compile

alias run_Plotter=${path_executable_Plotter}
alias run_basicPlotter=${path_executable_basicPlotter}
alias compile=${path_compile}


echo -e "------------------------------------------------ \n"

echo "per compilare il pacchetto, da qualunque sottocartella                         --> compile"

echo "per lanciare il plotter                                                        --> run_Plotter "
echo "per lanciare il plotter <basic>                                                --> run_basicPlotter "

echo -e "------------------------------------------------ \n"
echo -e "------------------------------------------------ \n"

echo " lsetup cmake..."
cd build
lsetup cmake

cd ..
echo -e "------------------------------------------------ \n"

