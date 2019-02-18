SUBDIRS := $(wildcard */.)

all: $(SUBDIRS)

compile: $(SUBDIRS)
	$(MAKE) -C $<

$(SUBDIRS):
	$(MAKE) clean -C $@

.PHONY: all $(SUBDIRS)
