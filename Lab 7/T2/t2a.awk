#! /usr/bin/awk -f
# Prints average grades of math reading and writing 
BEGIN {
	FS = ","
}

{
	if (NR > 1 && $4 == "standard") {
		cntStandard++
		avgMath = avgMath + $6
		avgReading = avgReading + $7
		avgWriting = avgWriting + $8
	}	
}

END {
	printf "Average Math grade: %i\n", avgMath/cntStandard
	printf "Average Reading grade: %i\n", avgReading/cntStandard
	printf "Average Writing grade: %i\n", avgWriting/cntStandard
}
