#
# You will need the rpm-devel package to compile this.
#

PYTHON_VERSION:=$(shell python -V 2>&1  | cut -d' ' -f2 | cut -d. -f1,2)

all:	_rpmext.so

_rpmext.so:	install.c
	$(CC) -Wall -I/usr/include/python$(PYTHON_VERSION) -fPIC -rdynamic --shared -o $@ -lrpm install.c

clean:
	rm -f _rpmext.so *.o
