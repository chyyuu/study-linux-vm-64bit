
ifneq ($(KERNELRELEASE),)
	obj-m := phy_mem.o

else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: default
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)  modules

install:
	cp -f phy_mem.ko ../../output/


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers

endif
