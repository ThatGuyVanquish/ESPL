#! /usr/bin/awk -f

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
