SUBDIRS := $(wildcard */.)
CMD     := --silent clean
# CMD     := all

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) $(CMD) -C $@
