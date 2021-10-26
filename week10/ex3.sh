echo Ilia Milioshin > _ex3.txt
chmod a=rw _ex3.txt
ls -l _ex3.txt > ex3.txt

chmod a=rwx _ex3.txt
chmod g=r _ex3.txt
ls -l _ex3.txt >> ex3.txt

chmod g=rwx _ex3.txt
ls -l _ex3.txt >> ex3.txt

echo "1) 660 stands for read and write permissions for user and group, no permissions for other" >> ex3.txt
echo "2) 775 stands for all permissions for user and group, read and execution for other" >> ex3.txt
echo "3) 777 stands for all permissions for everyone" >> ex3.txt
