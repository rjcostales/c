# SUBDIRS := $(wildcard */.)
SUBDIRS := $(shell ls */Makefile */*/Makefile | sed 's/Makefile/./')

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) all -C $@
	$(MAKE) --silent clean -C $@

.PHONY: all $(SUBDIRS)
