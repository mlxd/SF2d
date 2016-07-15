Two dimensional structure factor from vortex positions.

Drop this into a directory with vortex coordinates in files labell "vort_arr_xxx", where xxx is the file number, and it will generate Gnuplot -> tikz -> PDF images. The code assumes columns 2,4 are the coordinates, and saves everything in a xxx_z.csv output file after processing with SF.

Requires Gnuplot, pdflatex, as well as an OpenMP supported compiler. The gnuplot colormaps are sourced from here https://github.com/Gnuplotting/gnuplot-palettes 
