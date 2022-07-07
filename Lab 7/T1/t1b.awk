#! /usr/bin/awk -f
# Print all the actors in oscar_age_male as NAME | AGE | YEAR OF MOVIE
BEGIN {
	FS = ","
	OFS = " | "
}

{
	if (NR > 1)
		print $4, $3, $2
}
