
SUBDIRS =  misc-progs misc-modules \
           skull scull scullc scullp \
	   short shortprint pci simple usb lddbus
		 #sculld scullv sbull snull tty 

all: subdirs

subdirs:
	for n in $(SUBDIRS); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIRS); do $(MAKE) -C $$n clean; done
