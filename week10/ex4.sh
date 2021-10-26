if [ ! -d "tmp" ]; then
        mkdir tmp
fi

if [ ! -d "tmp/file1" ]; then
        echo > tmp/file1
fi

if [ ! -d "tmp/file2" ]; then
        echo > tmp/file2
fi

if [ ! -e "tmp/link1" ]; then
	ln tmp/file1 tmp/link1
fi
