
ifneq ($(KERNELRELEASE),)
	obj-m := sys_reg.o

else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: default
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)  modules

install:
	cp -f sys_reg.ko ../../output/


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers

endif
