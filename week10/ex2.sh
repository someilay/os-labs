if [ ! -d "week01" ]; then
	mkdir week01
fi

echo Ilia Milioshin > week01/file.txt

if [ ! -d "week10" ]; then
        mkdir week10
fi

link week01/file.txt week10/_ex2.txt
OUTPUT=$(ls -iQ1 week01/file.txt | awk '{print $1}')
find -inum $OUTPUT > ex2.txt
find -inum $OUTPUT -exec rm {} \; >> ex2.txt
