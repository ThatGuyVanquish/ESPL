#! /usr/bin/awk -f
# Print all the lines which have 'happy'
BEGIN {
	FS = ","
	OFS = "---"
}

{
	if ($2~/\<happy\>/)
	{
		print $2,$3,$4
		cnt++
	}
}

END {
	printf("There are %i 1st parts where it says 'happy'\n", cnt)
}
