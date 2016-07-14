all: SF TIKZ
	./SF 1000 999000
	gnuplot  -e "sf='1000_z.csv'" plot.gp
	gnuplot  -e "sf='999000_z.csv'" plot.gp
	pdflatex ./stand.tex
	open ./stand.pdf
	
SF: calcSF.cpp
	g++-mp-5 ./calcSF.cpp -o ./SF -fopenmp -std=c++11

pdf: stand.tex
	cat ./stand.tex | sed 's/myfilename/1000_pic.tikz/g' | pdflatex 
	mv texput.pdf SF_1000.pdf


LOOP: SF
	num=1000; while [[ $$num -lt 1000000 ]] ; do \
        	((num = num+1000)) ; \
		echo $${num} ; \
		./SF $$num ; \
		gnuplot -e "sf='$${num}_z.csv'; num='$${num}'" plot.gp ; \
		cat ./stand.tex | sed 's/myfilename/$${num}_pic.tikz/g' | pdflatex ; \
		mv texput.pdf SF_$${num}.pdf ; \
	done
