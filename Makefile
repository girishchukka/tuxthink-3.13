obj-m += c1.o
VER = $(shell uname -r)
all:
	make -C /lib/modules/$(VER)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(VER)/build M=$(shell pwd) clean
