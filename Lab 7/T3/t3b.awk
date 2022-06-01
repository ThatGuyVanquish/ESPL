#! /usr/bin/awk -f

BEGIN {
	FS = ","
	OFS = "---"
}

{
	if ($2~/\<fun\>/ || $3~/\<fun\>/ || $4~/\<fun\>/ || $2~/\<fan\>/ || $3~/\<fan\>/ || $4~/\<fan\>/)
		cnt++
}

END {
	printf "The number of poems that have fun or fan is: %i\n", cnt
}
