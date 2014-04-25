ACLOCAL_AMFLAGS = -I m4
SUBDIRS         = libjalali src man

if WANT_PYJALALI
install-exec-hook:
	@echo -e "\n###########################\n"\
"##  Installing Pyjalali  ##\n"\
"###########################\n"
	test -d pyjalali || mkdir pyjalali
	@$(CP) $(srcdir)/pyjalali/*.py pyjalali
	@$(CP) $(srcdir)/setup.py .
	LIBJALALI_DIR=$(DESTDIR)$(libdir) \
	$(PYTHON) setup.py install --prefix=$(DESTDIR)$(exec_prefix) --record=pyinstalled.txt

uninstall-hook:
	while read -r fn; do $(RM) -rf "$$fn"; done <pyinstalled.txt
	-$(RMDIR) $(DESTDIR)$(pythondir)/pyjalali && $(RM) pyinstalled.txt
endif
