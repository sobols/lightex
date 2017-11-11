#include <lightex/pymod/pylightex.h>

#include <lightex/pymod/extension.h>
#include <lightex/pymod/cpython.h>

#include <stdexcept>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

#define MODULE_DOC "LighTeX-based TeX -> HTML converter library"

/**
* module state
*/

// Python does not support module unloading,
// so nobody deletes this pointer :(
lightex::extension::Module* gModule;

static PyObject* LighTeXError;
static PyTypeObject lightex_ResultType;

/**
* Context
*/

typedef struct {
  PyObject_HEAD
    lightex::extension::Context* ctxt;
} lightex_ContextObject;

static int lightex_ContextObject_init(lightex_ContextObject* self, PyObject* args, PyObject* kwds) {
  int builtinKaTeX = 0;

  static char* kwlist[] = { "builtin_katex", NULL };

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &builtinKaTeX)) {
    return -1;
  }

  if (gModule == NULL) {
    PyErr_SetString(LighTeXError, "module was not properly initialized");
    return -1;
  }

  lightex::extension::Context* newCtxt;
  try {
    newCtxt = gModule->CreateContext(builtinKaTeX).release();
  } catch (const std::exception& ex) {
    PyErr_SetString(LighTeXError, ex.what());
    return -1;
  }

  delete self->ctxt;
  self->ctxt = newCtxt;

  return 0;
}

static void lightex_ContextObject_dealloc(lightex_ContextObject* self) {
  delete self->ctxt;
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* lightex_ContextObject_execute(lightex_ContextObject* self, PyObject* args) {
  const char* inputData = NULL;
  Py_ssize_t inputLength = 0;

  if (!PyArg_ParseTuple(args, "s#", &inputData, &inputLength)) {
    return NULL;
  }

  lightex::extension::Result ltRes;
  try {
    std::string input(inputData, inputLength);
    ltRes = self->ctxt->Execute(input);
  } catch (const std::exception& ex) {
    PyErr_SetString(LighTeXError, ex.what());
    return NULL;
  }

  PyObject* pyRes = PyStructSequence_New(&lightex_ResultType);
  if (pyRes == NULL) {
    return NULL;
  }

  PyStructSequence_SET_ITEM(pyRes, 0, PyBool_FromLong(ltRes.ok));
  PyStructSequence_SET_ITEM(pyRes, 1, PyUnicode_FromStringAndSize(ltRes.html.c_str(), ltRes.html.size()));
  PyStructSequence_SET_ITEM(pyRes, 2, PyUnicode_FromStringAndSize(ltRes.error.c_str(), ltRes.error.size()));

  if (PyErr_Occurred()) {
    Py_DECREF(pyRes);
    return NULL;
  }

  return pyRes;
}

static PyMethodDef lightex_Context_methods[] = {
  { "execute", (PyCFunction)lightex_ContextObject_execute, METH_VARARGS, "Execute the TeX code" },
  { NULL } /* sentinel */
};

static PyTypeObject lightex_ContextType = {
  PyVarObject_HEAD_INIT(NULL, 0) "pylightex.Context", /* tp_name */
  sizeof(lightex_ContextObject),                      /* tp_basicsize */
  0,                                                  /* tp_itemsize */
  (destructor)lightex_ContextObject_dealloc,          /* tp_dealloc */
  0,                                                  /* tp_print */
  0,                                                  /* tp_getattr */
  0,                                                  /* tp_setattr */
  0,                                                  /* tp_compare */
  0,                                                  /* tp_repr */
  0,                                                  /* tp_as_number */
  0,                                                  /* tp_as_sequence */
  0,                                                  /* tp_as_mapping */
  0,                                                  /* tp_hash */
  0,                                                  /* tp_call */
  0,                                                  /* tp_str */
  0,                                                  /* tp_getattro */
  0,                                                  /* tp_setattro */
  0,                                                  /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,                                 /* tp_flags */
  "LighTeX rendering context",                        /* tp_doc */
  0,                                                  /* tp_traverse */
  0,                                                  /* tp_clear */
  0,                                                  /* tp_richcompare */
  0,                                                  /* tp_weaklistoffset */
  0,                                                  /* tp_iter */
  0,                                                  /* tp_iternext */
  lightex_Context_methods,                            /* tp_methods */
  0,                                                  /* tp_members */
  0,                                                  /* tp_getset */
  0,                                                  /* tp_base */
  0,                                                  /* tp_dict */
  0,                                                  /* tp_descr_get */
  0,                                                  /* tp_descr_set */
  0,                                                  /* tp_dictoffset */
  (initproc)lightex_ContextObject_init,               /* tp_init */
  0,                                                  /* tp_alloc */
  0,                                                  /* tp_new */
};

/**
* Result
*/

static PyStructSequence_Field lightex_ResultType_fields[] = {
  { "ok", "Success" },
  { "html", "HTML data" },
  { "error", "Error string" },
  { NULL }
};

static PyStructSequence_Desc lightex_ResultType_desc = {
  "pylightex.Result",
  "TeX execution result",
  lightex_ResultType_fields,
  3
};

/**
* module
*/

/* List of functions defined in the module */
static PyMethodDef lightex_methods[] = {
  { NULL } /* sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef moduledef = {
  PyModuleDef_HEAD_INIT,
  "pylightex",
  MODULE_DOC,
  -1,
  lightex_methods,
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_pylightex(void)
#else

#define INITERROR return

PyMODINIT_FUNC
initpylightex(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
  PyObject* m = PyModule_Create(&moduledef);
#else
  PyObject* m = Py_InitModule3("pylightex", lightex_methods, MODULE_DOC);
#endif

  if (m == NULL) {
    INITERROR;
  }

  // context
  lightex_ContextType.tp_base = &PyBaseObject_Type;
  lightex_ContextType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&lightex_ContextType) < 0) {
    INITERROR;
  }
  Py_INCREF(&lightex_ContextType);
  PyModule_AddObject(m, "Context", (PyObject*)&lightex_ContextType);

  // result
  if (lightex_ResultType.tp_name == NULL) {
    PyStructSequence_InitType(&lightex_ResultType, &lightex_ResultType_desc);
  }
  Py_INCREF(&lightex_ResultType);
  PyModule_AddObject(m, "Result", (PyObject*)&lightex_ResultType);

  // exception
  if (LighTeXError == NULL) {
    LighTeXError = PyErr_NewException("pylightex.error", NULL, NULL);
    if (LighTeXError == NULL) {
      INITERROR;
    }
  }
  Py_INCREF(LighTeXError);
  PyModule_AddObject(m, "error", LighTeXError);

  try {
    gModule = new lightex::extension::Module();
  } catch (const std::exception& ex) {
    PyErr_SetString(LighTeXError, ex.what());
  }

#if PY_MAJOR_VERSION >= 3
  return m;
#endif
}
