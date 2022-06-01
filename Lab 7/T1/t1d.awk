#! /usr/bin/awk -f

BEGIN {
	FS = ","
}

{
	if (NR > 1 && $1 >= 50 && $1 <= 70) {
		printf "Movie Name: %s\n", $5
		print "—----------------------------------"
		cnt++
	}
		
}

END {
	printf "Counted %i movies\n", cnt
}
