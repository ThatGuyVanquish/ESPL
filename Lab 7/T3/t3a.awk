#! /usr/bin/awk -f
#Task3a Print all the poems with 'spring' in them

BEGIN {
	FS = ","
	OFS = "---"
}

{
	if ($2~/spring/ || $3~/spring/ || $4~/spring/)
		print $2,$3,$4
}

END {
}

#! /usr/bin/awk -f
#Task3c Print all the lines which have 'happy'
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
