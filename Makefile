#!Makefile

CFLAGS = -Wall -g

SUBDIRS := \
	modules/sys_reg \
	modules/phy_mem \
	apps/running-prog \
	apps/read-phy-mem \



all:
	for dir in $(SUBDIRS);\
        do $(MAKE) -C $$dir all ; \
		echo "";\
        done
install:
	mkdir -p output
	for dir in $(SUBDIRS);\
	do $(MAKE) -C $$dir install ;\
	echo "";\
	done

clean:
	rm -rf output
	@for dir in $(SUBDIRS); do make -C $$dir clean ; echo ""; done


.PHONY:dropbox
dropbox:
	./tools/addDropboxExclude.sh

.PHONY:update_files
update_files:
	git ls-files "*Makefile" "*.c" "*.h" "*.s" > study-linux-vm-32bit.files
