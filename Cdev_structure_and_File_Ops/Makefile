
obj-m +=Cdev_struct_and_Fops.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -c $(KDIR) M=$(shell pwd) clean
