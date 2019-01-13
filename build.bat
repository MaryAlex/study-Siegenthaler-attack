@echo off

cd "C:\Users\Maksim\Desktop\study-Siegenthaler-attack-master"

mkdir .build
pushd .build
cmake -G "Visual Studio 12 2013 Win64" ..
popd

pause