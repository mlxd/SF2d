#!/bin/bash
#g++-mp-5 ./calcSF.cpp -o ./SF -fopenmp -std=c++11
for ii in {0..1000..1000};do
	./SF $ii
	filename="sq_${ii}.png"
	dataname="Sq_${ii}.csv"
	gnuplot -e "filename=${filename}; dataName=${dataname}" ./plot.gp

done
