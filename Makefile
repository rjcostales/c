# SUBDIRS := $(wildcard */.)
SUBDIRS := $(shell ls */Makefile */*/Makefile | sed 's/Makefile/./')

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) --silent clean -C $@
	$(MAKE) -C $@
	$(MAKE) --silent clean -C $@

.PHONY: all $(SUBDIRS)

clean:
