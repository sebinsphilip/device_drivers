obj-m += query_ioctl.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o query_app query_app.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -fr query_app
