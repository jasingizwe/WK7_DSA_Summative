#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <stdlib.h>

// Helper: Convert Python list to C array
int parse_float_array(PyObject *input_list, double **array, Py_ssize_t *length) {
    if (!PyList_Check(input_list)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list of floats.");
        return 0;
    }

    *length = PyList_Size(input_list);
    *array = (double *)malloc(*length * sizeof(double));
    if (!*array) {
        PyErr_NoMemory();
        return 0;
    }

    for (Py_ssize_t i = 0; i < *length; i++) {
        PyObject *item = PyList_GetItem(input_list, i);
        if (!PyFloat_Check(item)) {
            free(*array);
            PyErr_SetString(PyExc_TypeError, "List must contain only float values.");
            return 0;
        }
        (*array)[i] = PyFloat_AsDouble(item);
    }
    return 1;
}

// sum_values
static PyObject* sum_values(PyObject* self, PyObject* args) {
    PyObject *input_list;
    if (!PyArg_ParseTuple(args, "O", &input_list))
        return NULL;

    double *values;
    Py_ssize_t len;
    if (!parse_float_array(input_list, &values, &len))
        return NULL;

    double sum = 0.0;
    for (Py_ssize_t i = 0; i < len; i++)
        sum += values[i];

    free(values);
    return Py_BuildValue("d", sum);
}

// mean_values
static PyObject* mean_values(PyObject* self, PyObject* args) {
    PyObject *input_list;
    if (!PyArg_ParseTuple(args, "O", &input_list))
        return NULL;

    double *values;
    Py_ssize_t len;
    if (!parse_float_array(input_list, &values, &len))
        return NULL;

    double sum = 0.0;
    for (Py_ssize_t i = 0; i < len; i++)
        sum += values[i];

    double mean = sum / len;
    free(values);
    return Py_BuildValue("d", mean);
}

// std_dev
static PyObject* std_dev(PyObject* self, PyObject* args) {
    PyObject *input_list;
    if (!PyArg_ParseTuple(args, "O", &input_list))
        return NULL;

    double *values;
    Py_ssize_t len;
    if (!parse_float_array(input_list, &values, &len))
        return NULL;

    double sum = 0.0, mean, std = 0.0;
    for (Py_ssize_t i = 0; i < len; i++)
        sum += values[i];
    mean = sum / len;

    for (Py_ssize_t i = 0; i < len; i++)
        std += pow(values[i] - mean, 2);

    std = sqrt(std / (len - 1));
    free(values);
    return Py_BuildValue("d", std);
}

// count_values
static PyObject* count_values(PyObject* self, PyObject* args) {
    PyObject *input_list;
    if (!PyArg_ParseTuple(args, "O", &input_list))
        return NULL;

    Py_ssize_t len = PyList_Size(input_list);
    return Py_BuildValue("n", len);
}

// mode_value
static PyObject* mode_value(PyObject* self, PyObject* args) {
    PyObject *input_list;
    if (!PyArg_ParseTuple(args, "O", &input_list))
        return NULL;

    double *values;
    Py_ssize_t len;
    if (!parse_float_array(input_list, &values, &len))
        return NULL;

    double mode = values[0];
    int max_count = 0;

    for (Py_ssize_t i = 0; i < len; i++) {
        int count = 1;
        for (Py_ssize_t j = i + 1; j < len; j++) {
            if (values[j] == values[i])
                count++;
        }
        if (count > max_count) {
            max_count = count;
            mode = values[i];
        }
    }

    free(values);
    return Py_BuildValue("d", mode);
}

// Method table
static PyMethodDef StatsMethods[] = {
    {"sum_values", sum_values, METH_VARARGS, "Return the sum of float values"},
    {"mean_values", mean_values, METH_VARARGS, "Return the mean of float values"},
    {"std_dev", std_dev, METH_VARARGS, "Return the sample standard deviation"},
    {"mode_value", mode_value, METH_VARARGS, "Return the mode (most frequent value)"},
    {"count_values", count_values, METH_VARARGS, "Return count of values"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef statsmodule = {
    PyModuleDef_HEAD_INIT,
    "stats_module",
    "Statistics functions in C",
    -1,
    StatsMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_stats_module(void) {
    return PyModule_Create(&statsmodule);
}
