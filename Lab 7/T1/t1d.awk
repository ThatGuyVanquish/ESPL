#! /usr/bin/awk -f
# Print all the movies where the index is between 50 and 70
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
