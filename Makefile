SUBDIRS := $(wildcard */.)

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) --silent clean -C $@
#	$(MAKE) all -C $@
