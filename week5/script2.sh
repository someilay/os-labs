
if [ ! -f num.txt ]; then
	echo 0 > num.txt
fi

counter=0
while [ $counter -lt 100 ]; do
	if ln num.txt num.lock ; then
		counter=$(expr $counter + 1)
		value=$(tail -1 num.txt)
		value=$(expr $value + 1)
		echo $value >> num.txt
		rm num.lock
	fi
done
