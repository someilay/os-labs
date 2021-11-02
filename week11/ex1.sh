RES=$(losetup -a | grep lofs.img | awk '{print substr($1, 1, length($1)-1)}')
if [ ! $RES = '' ]; then
	if [ -e 'lofsdisk' ]; then
		sudo umount lofsdisk
		sudo rm -R lofsdisk
	fi
	if [ -e 'lofs.img' ]; then
		sudo losetup -d $RES
		sudo rm lofs.img
	fi
fi

fallocate -l 50M lofs.img
sudo losetup -f lofs.img
RES=$(losetup -a | grep lofs.img | awk '{print substr($1, 1, length($1)-1)}')
sudo mkfs.ext4 $RES
mkdir lofsdisk
sudo mkfs.ext4 $RES
sudo mount $RES lofsdisk
