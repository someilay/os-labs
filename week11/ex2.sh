if [ ! -e 'lofsdisk' ]; then
	sh ex1.sh
fi

sudo bash -c "echo Ilia > lofsdisk/file1"
sudo bash -c "echo Milioshin > lofsdisk/file2"


BASH_LIBS=$(ldd /bin/bash | egrep -o '/lib.*\.[0-9]')
CAT_LIBS=$(ldd '/bin/cat' | egrep -o '/lib.*\.[0-9]')
ECHO_LIBS=$(ldd '/bin/echo' | egrep -o '/lib.*\.[0-9]')
LS_LIBS=$(ldd /bin/ls | egrep -o '/lib.*\.[0-9]')
ALL_LIBS="$BASH_LIBS $CAT_LIBS $ECHO_LIBS $LS_LIBS"

for libs in $ALL_LIBS; do
	PATH_TO=$(dirname $libs)
	sudo mkdir -p lofsdisk$PATH_TO && sudo cp $libs lofsdisk$PATH_TO
done

sudo mkdir -p lofsdisk/bin && sudo cp /bin/bash lofsdisk/bin
sudo mkdir -p lofsdisk/bin && sudo cp '/bin/cat' lofsdisk/bin
sudo mkdir -p lofsdisk/bin && sudo cp '/bin/echo' lofsdisk/bin
sudo mkdir -p lofsdisk/bin && sudo cp /bin/ls lofsdisk/bin

if [ ! -e ex2.out ]; then
	gcc ex2.c -o ex2.out
fi

sudo cp ex2.out lofsdisk
echo 'Output of ex2.out when root is lofsdisk:' > ex2.txt
sudo chroot lofsdisk /ex2.out >> ex2.txt
echo '\nOutput of ex2.out when root is /' >> ex2.txt
./ex2.out >> ex2.txt
