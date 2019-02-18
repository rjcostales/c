SUBDIRS := $(wildcard */.)
CMD     := clean

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) $(CMD) -C $@

.PHONY: all $(SUBDIRS)
