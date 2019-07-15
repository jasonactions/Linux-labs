obj-m := demo_chr_dev.o
KERNELDIR := /root/HZero/os/linux-4.0
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -f *.o *.ko *.mod.c
.PHNEY: clean
