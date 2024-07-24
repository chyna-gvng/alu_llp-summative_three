#include <Python.h>
#include <stdio.h>

#define PY_SSIZE_T_CLEAN

int main() {
    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
    
    Py_Initialize();

    // Add the current directory to Python's path
    PyRun_SimpleString("import sys; sys.path.append('.')");

    pName = PyUnicode_DecodeFSDefault("temperature_stats");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "calculate_stats");

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);
            pValue = PyUnicode_DecodeFSDefault("temperature.csv");
            PyTuple_SetItem(pArgs, 0, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != NULL) {
                double mean, mode, std_dev;
                PyArg_ParseTuple(pValue, "ddd", &mean, &mode, &std_dev);
                printf("Mean temperature: %.2f K\n", mean);
                printf("Mode temperature: %.2f K\n", mode);
                printf("Standard deviation: %.2f K\n", std_dev);
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return 1;
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function 'calculate_stats'\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load 'temperature_stats'\n");
        return 1;
    }

    Py_Finalize();
    return 0;
}
