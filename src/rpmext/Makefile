#
# You will need the boost python library and the rpm-devel package to compile this.
#
_rpmext.so:	install.cxx
	$(CXX) -I/usr/include/python2.6 -fPIC -rdynamic --shared -o $@ -lboost_python -lrpm install.cxx

clean:
	rm -f _rpmext.so *.o
