
#include <boost/python.hpp>
#include <rpm/rpmfi.h>
#include <rpm/rpmts.h>

namespace {

    // A literal copy from rpm/python/rpmts-py.c rpm-4.8.0
    struct rpmfdObject;
    struct rpmtsObject_s {
        PyObject_HEAD
        PyObject *md_dict;
        rpmfdObject *scriptFd;
        PyObject *keyList;
        rpmts	ts;
        // rpmtsi tsi;
    };
    typedef struct rpmtsObject_s rpmtsObject;

    struct hdrObject_s {
        PyObject_HEAD
        Header h;
        HeaderIterator hi;
    };
    typedef hdrObject_s hdrObject;

    bool rpmts_AddInstall(PyObject *ts, PyObject* header, PyObject *key, int how, const char *oldpath, const char *newpath)
    {
        // brute force, assumes that we run against the correct rpm version
        rpmtsObject *s = reinterpret_cast<rpmtsObject*>(ts);
        Header h =  reinterpret_cast<hdrObject*>(header)->h;
        
        rpmRelocation *relocs = 0;

        if(oldpath != 0 && newpath != 0) {
            relocs = reinterpret_cast<rpmRelocation*>(calloc(2,sizeof(rpmRelocation)));
            relocs[0].oldPath = strdup(oldpath);
            relocs[0].newPath = strdup(newpath);
            relocs[1].oldPath = 0;
            relocs[1].newPath = 0;
        }

        int rc = rpmtsAddInstallElement(s->ts, h, key, how, relocs);
        if (key && rc == 0) {
            PyList_Append(s->keyList, key);
        }

        return (rc == 0);
    }

}

BOOST_PYTHON_MODULE(_rpmext)
{
    using namespace boost::python;
    def("addInstall", rpmts_AddInstall);
}
