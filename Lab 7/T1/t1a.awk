#Task 1a

#! /usr/bin/awk -f
# Print argv
BEGIN {
	for (i = 0; i < ARGC; i++)
		printf "ARGV[%d] = %s\n", i, ARGV[i]
}

#Task 1b

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

#Task 1c

#! /usr/bin/awk -f
# Print all actors who played in movies that were shot after 1970
BEGIN {
	FS = ","
}

{
	if (NR > 1 && $2 >= 1970) {
		printf "Actor Name: %s\n", $4
		printf "Movie Name: %s\n", $5
		print "—----------------------------------"
	}
		
}

#Task 1d

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
