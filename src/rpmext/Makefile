#
# You will need the boost python library and the rpm-devel package to compile this.
#
all:	_rpmext.so

_rpmext.so:	install.c
	$(CC) -Wall -I/usr/include/python2.6 -fPIC -rdynamic --shared -o $@ -lrpm install.c

clean:
	rm -f _rpmext.so *.o
