#! /usr/bin/awk -f
# Print information about the students with averages over 80
BEGIN {
	FS = ","
	print ("=======")
	print ("\"Success Student List\"")
	print ("=======")
}

{
	if (NR > 1 && $6 >= 80 && $7 >= 80 && $8 >= 80) {
		cnt++
		print $0
	}	
}

END {
	printf "The number of excelling students: %i\n", cnt
}
