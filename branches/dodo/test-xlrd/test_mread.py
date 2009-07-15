#test for mread module
import mread
print "Line 0 : ",mread.read_line("model_small.xls",0,0)
print "Column 0 : ",mread.read_col("model_small.xls",0,0)
print "Column (Acoperit)",mread.read_coln("model_small.xls",0,"Acoperit")
