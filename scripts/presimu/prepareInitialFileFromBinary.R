#######################################################################
##
## Read txt file of the position of binary-positive pixels in an image
## And place cells in this area
##
## Creation: June 2017, G. Letort, Institut Curie
##
######################################################################

## input file in which positive pixels are listed (output from ImageJ by e.g.)
datas = read.table("in.txt")

x = datas[,1]
y = datas[,2]

# recentre datas in (0,0)
x = (x - mean(x)) 
y = (y - mean(y)) 

df = c()
for (i in c(1:length(x)) )
{
	## don't keep all pixels (one cell can take more than one pixel)
    if ( i%%10 == 0 )
    {
		## cell line of the cell to add
		type = 0
		## initial phase of the cell
		ph = 3
		## write values for one cell:
		## 0-Time, 1-ID, 2-x, 3-y, 4-z, 5-radius, 6-11 -1 doesn't have to be filled, 12-cell line number, 13-cell phase, 14-time elapsed in this cell phase, 15-16 doesn't have ot be filled
        sphere = c( 0, 0, x[i], y[i], rnorm(1,0,8), runif(1,3.4,4.5), -1, -1, -1, -1, -1, -1, type, ph, 0, -1 )
        df = rbind( df, sphere )
    }
}

## write resulting initial file. Can then be used directly in a simulation
df = data.frame( df )
write.table( file="out.txt", df, sep=";", row.names=F )
