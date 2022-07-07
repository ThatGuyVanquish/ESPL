#! /usr/bin/awk -f
#Print the number of people who have bachelor's degree (not counting masters)

BEGIN {
	FS = ","
}

{
	if (NR > 1 && $3 == "bachelor's degree") {
		cnt++
	}	
}

END {
	printf "Amount of bachelors degrees: %i\n", cnt
}
