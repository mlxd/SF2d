load 'gnuplot-palettes/parula.pal'
set term tikz
set output num."_pic.tikz"
set size ratio 1
set pm3d map
#set cbrange[0:5000]
set colorbox user
set clabel "$S(\\mathbf{q})$"
#unset colorbox
set xlabel "$q_x$"
set ylabel "$q_y$"
set xrange [-9:9]
set yrange [-9:9]
set xtics ("$-3\\pi$" -3*pi, "$-2\\pi$" -2*pi, "$-\\pi$" -pi, "$0$" 0, "$\\pi$" pi, "$2\\pi$" 2*pi, "$3\\pi$" 3*pi)
set ytics ("$-3\\pi$" -3*pi, "$-2\\pi$" -2*pi, "$-\\pi$" -pi, "$0$" 0, "$\\pi$" pi, "$2\\pi$" 2*pi, "$3\\pi$" 3*pi)
#set xtics -2*pi,pi,2*pi
#set ytics -2*pi,pi,2*pi
#splot sf u ($1 <=1 && $1 >=-1 ? 1/0 : $1):2:3 notitle
set object 1 rect from -1,-1 to 1,1 fc rgb "black"
splot sf u 1:2:($1 <=1 && $1 >=-1 && $2 <=1 && $2 >= -1 ? 1/0 : $3) notitle
#splot sf u 1:2:3 notitle
