/*******************************************************************************
 * Copyright (c) 2001-2020 Rohit Priyadarshi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <gtest/gtest.h>

// https://www.codeproject.com/Articles/820116/Embedding-Python-program-in-a-C-Cplusplus-code
// https://docs.python.org/3.8/extending/embedding.html
// https://stackoverflow.com/questions/16545297/c-python-import-class-call-methods
// https://codereview.stackexchange.com/questions/196891/embedding-python-into-c-program
// https://stackoverflow.com/questions/39813301/creating-a-python-object-in-c-and-calling-its-method
// make sure PYTHONPATH also has "." in the path
// https://stackoverflow.com/questions/13422764/why-does-pyimport-import-fail-to-load-a-module-from-the-current-directory
#include <Python.h>
#include <filesystem>

TEST(compileTest, multistring)
{
    Py_Initialize();

    PySys_SetPath(std::filesystem::current_path().wstring().data());

      PyObject* mod_name = PyUnicode_FromString("pyutl");
    EXPECT_TRUE(mod_name != nullptr);

    PyObject* mod_ptr = PyImport_Import(mod_name);
    EXPECT_TRUE(mod_ptr != nullptr);
      Py_DECREF(mod_name);

      PyObject* dict_ptr = PyModule_GetDict(mod_ptr);
    EXPECT_TRUE(dict_ptr != nullptr);
      Py_DECREF(mod_ptr);

      PyObject* class_ptr = PyDict_GetItemString(dict_ptr, "clMultiString");
    EXPECT_TRUE(class_ptr != nullptr);
      Py_DECREF(dict_ptr);

    PyObject* obj_ptr = nullptr;
    if (PyCallable_Check(class_ptr)) {
        obj_ptr = PyObject_CallObject(class_ptr, nullptr);
        Py_DECREF(class_ptr);
    }
    Py_DECREF(class_ptr);
    EXPECT_TRUE(obj_ptr != nullptr);

    PyObject *val_ptr = PyObject_CallMethod(obj_ptr, "size", "()"); 
    EXPECT_TRUE(val_ptr != nullptr);
    uint64_t val = PyLong_AsLong(val_ptr);
    Py_DECREF(val_ptr);

    EXPECT_EQ(0u, val);

    Py_Finalize();
}
