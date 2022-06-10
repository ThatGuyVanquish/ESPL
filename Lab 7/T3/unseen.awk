#! /usr/bin/awk -f

BEGIN {
	FS = ","
	OFS = "---"
}

{
	n = split($2, sep, " ")
	if (n < 3) {
		print $2
		cnt++
	}
	cnt2 = 0
}

END {
	printf "The number of poems that have less or 3 words in the 2nd part is: %i\n", cnt
}