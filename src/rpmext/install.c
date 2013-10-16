
#include <Python.h>
#include <rpm/rpmfi.h>
#include <rpm/rpmts.h>

/* A literal copy from rpm/python/rpmts-py.c rpm-4.8.0 */
struct rpmfdObject;
struct rpmtsObject_s {
    PyObject_HEAD
    PyObject *md_dict;
    struct rpmfdObject *scriptFd;
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

typedef struct hdrObject_s hdrObject;

/* PyObject *ts, PyObject* header, PyObject *key, int how */
static PyObject *
rpmts_AddInstall(PyObject *self, PyObject * args)
{
    PyObject *ts = NULL;
    PyObject *header = NULL;
    PyObject * key;
    int how = 0;
    PyObject *relocations = NULL;

    rpmtsObject *s = NULL;
    Header h = NULL;
    int rc;
    rpmRelocation *relocs = NULL;

    if (!PyArg_ParseTuple(args, "OOOiO:AddInstall", 
			  &ts, &header, &key, &how, &relocations))
	return NULL;

    /* unchecked conversions */
    s = (rpmtsObject *)ts;
    h = ((hdrObject *)header)->h;

    if(relocations && relocations != Py_None) {
        Py_ssize_t list_size;
        Py_ssize_t i;

        if(!PyList_Check(relocations)) {
	    PyErr_SetString(PyExc_TypeError, "expected a list for relocations");
            return NULL;
        }

        // each list entry is a tuple(oldpath,newpath)
        list_size = PyList_Size(relocations);
        relocs = (rpmRelocation *)PyMem_Malloc((list_size + 1) * sizeof(rpmRelocation));
        for(i = 0; i < list_size; i++) {
            PyObject *oldpath, *newpath;
            PyObject *item = PyList_GetItem(relocations, i);
            if(!PyTuple_Check(item) || PyTuple_Size(item) != 2) {
                PyErr_SetString(PyExc_TypeError, "expected a tuple with two strings");
                return NULL;
            }
            oldpath = PyTuple_GetItem(item, 0);
            newpath = PyTuple_GetItem(item, 1);
            if(!PyString_Check(oldpath) || !PyString_Check(newpath)) {
                PyErr_SetString(PyExc_TypeError, "expected a tuple with two strings");
                return NULL;
            }
            relocs[i].oldPath = strdup(PyString_AsString(oldpath));
            relocs[i].newPath = strdup(PyString_AsString(newpath));
        }
        relocs[i].oldPath = NULL;
        relocs[i].newPath = NULL;
    }

    rc = rpmtsAddInstallElement(s->ts, h, key, how, relocs);
    if (key && rc == 0) {
	PyList_Append(s->keyList, key);
    }
    return PyBool_FromLong((rc == 0));
}

static PyMethodDef rpmext_methods[] = {
    {"addInstall",  rpmts_AddInstall, METH_VARARGS,
     "Add RPM install or update transaction."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
init_rpmext(void)
{
    Py_InitModule("_rpmext", rpmext_methods);
}
