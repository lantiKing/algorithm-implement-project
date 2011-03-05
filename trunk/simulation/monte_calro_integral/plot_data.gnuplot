##%gnuplot <this file>

#output png file (color)
set terminal png
set output "mci_result.png"

#output gif file (color)
#set terminal gif
#set output "mci_result.gif"

#output eps file (mono)
#set terminal postscript eps
#set output "mci_result.eps"

#set line style
set style line 1 lt 1 pt 1

#set figure title
set title "Monte Carlo Integral"


#(*)set range
set xrange [0.9:2.1]
set yrange [-0.1:1.1]

#set labels
set xlabel "x"
set ylabel "y"

#(*)plot graphes
f(x)=cos(log(x))
plot f(x) title "cos(loge(x))" with lines,\
			"mci.dat" using 1:2 with points title "Points"
