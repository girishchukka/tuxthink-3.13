obj-m += c1.o
obj-m += proc_entry_example.o
VER = $(shell uname -r)
all:
	make -C /lib/modules/$(VER)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(VER)/build M=$(shell pwd) clean
