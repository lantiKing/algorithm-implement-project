##%gnuplot <this file>

#output png file (color)
set terminal png
set output "buffon_result.png"

#output gif file (color)
#set terminal gif
#set output "buffon_result.gif"

#output eps file (mono)
#set terminal postscript eps
#set output "buffon_result.eps"

#set line style
set style line 1 lt 1 pt 1
set style line 2 lt 2 pt 2
set style line 3 lt 3 pt 5

#set figure title
set title "Buffon's Needle problem"

set y2tics nomirror

#(*)set range
set xrange [0:1000]

#set labels
set xlabel "n"
set ylabel "pi"
set y2label "Pr"

#(*)plot graphes
f(x)=pi
plot f(x) title "Real pi" with lines axis x1y1 linestyle 1,\
			"buffon.dat" using 1:3 with linespoints title "Calculate pi" axis x1y1 linestyle 2,\
			"buffon.dat" using 1:2 with linespoints title "Pr" axis x1y2 linestyle 3
