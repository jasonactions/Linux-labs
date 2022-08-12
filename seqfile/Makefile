obj-m   += seqfile_test.o

KDIR    := /lib/modules/$(shell uname -r)/build
#KDIR    := /home/ubuntu/qemu/kernel/linux
PWD     := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f modules.order

