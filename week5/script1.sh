
if [ ! -f num.txt ]; then
	echo 0 > num.txt
fi

counter=0

while [ $counter -lt 100 ]; do
	counter=$(expr $counter + 1)
	value=`tail -1 num.txt`
	value=$(expr $value + 1)
	echo $value >> num.txt
done
