#! /usr/bin/awk -f

BEGIN {
	FS = ","
	OFS = "---"
}

{
	if ($2~/\<happy\>/)
		print $2,$3,$4
}

END {
}
