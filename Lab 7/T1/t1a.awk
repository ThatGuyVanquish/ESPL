#! /usr/bin/awk -f
# Print argv
BEGIN {
	for (i = 0; i < ARGC; i++)
		printf "ARGV[%d] = %s\n", i, ARGV[i]
}
