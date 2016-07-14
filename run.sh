#!/bin/zsh
#g++-mp-5 ./calcSF.cpp -o ./SF -fopenmp -std=c++11
for ii in {0..20000..1000};do
	./SF ${ii}
	gnuplot -e "sf='${ii}_z.csv'; num='${ii}'" plot.gp
	cat ./stand.tex | sed "s/myfilename/${ii}_pic.tikz/g" | pdflatex 
	mv texput.pdf SF_${ii}.pdf 
	rm ${ii}_pic.tikz ${ii}_z.csv ${ii}_x.csv ${ii}_y.csv
done
rm texput.*
