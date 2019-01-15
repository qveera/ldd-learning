obj-m := uniModuleTasklet.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko .*.o .*.ko mo* Mo* *.mod.c .c* .t* test Module.symvers *.order *.markers .*.cmd)
