obj-m :=uniCharDevComplete.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	gcc uniReader.c -o reader
	gcc uniWriter.c -o writer

clean:
	rm -rf $(wildcard *.o *.ko .*.o .*.ko mo* Mo* *.mod.c .c* .t* test Module.symvers *.order *.markers .*.cmd reader writer .*.o.d)
