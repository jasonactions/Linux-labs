
obj-m += waitqueue.o

PWD = $(shell pwd)
KERNEL := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL) M=$(PWD) modules
clean:
	rm *.ko *.o *.mod.c *.order *.symvers
.PHONY: clean
