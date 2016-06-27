set term png
set output filename
set size ratio 1
set pm3d map
splot "z.csv" u 1:2:3
