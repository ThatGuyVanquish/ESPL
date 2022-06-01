#! /usr/bin/awk -f

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
