# commands

sudo insmod module_name.ko

sudo dmesg

sudo lsmod

# creating a device file
mknod /dev/myDev c 236 0

sudo rmmod module_name

# everytime when i finished editing mynull.c
sudo rmmod mynull && sudo insmod mynull.ko
sudo rm /dev/myNull && sudo mknod /dev/myNull c 236 0 && sudo chmod a+r+w /dev/myNull

echo "hello world" >>  /dev/myNull
cat /dev/myNull

python3 -c 'print("a"*90+"c"*20)' > /dev/myNull
cat /dev/myNull
