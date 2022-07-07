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
