# study-linux-vm-32bit

[study-linux-vm-32bit](https://github.com/ziyang3721/study-linux-vm-32bit)

本文是配合 [该文章](http://ziyang3721.github.io/2015/02/02/study-linux-vm-32bit.html)  使用.

# 使用方式
make  

make install


## 加载模块
sudo insmod ./output/sys_reg.ko

sudo insmod ./output/phy_mem.ko

## running-prog
./output/running-prog

## read-phy-mem
因为读写 /dev/phy_mem 设备的权限问题请使用 sudo 运行.

sudo ./read-phy-mem addr len 



# status
本程序在 i386 linux mint 14, kernel 3.5.0-17 模式下测试通过.

