timestamp="$(date +"%s")"

# for i in "1" "1.1" "1.2" "1.3"
for i in 1 "1.1"
do
	for j in "1.2" "1.25" "1.3"
	do
		for k in 2 3
		do
			for l in 1 3 
			do
				./PCG_assignment2 $i $j $k 257 $l $timestamp 1 &
			done
		done
	done
done
