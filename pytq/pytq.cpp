#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "tq/tq.hpp"
#include "tq/value.hpp"

namespace py = pybind11;

// Convert Python object to TQ Value
tq::Value python_to_value(py::object obj) {
    if (obj.is_none()) {
        return tq::Value();
    } else if (py::isinstance<py::bool_>(obj)) {
        return tq::Value(obj.cast<bool>());
    } else if (py::isinstance<py::int_>(obj)) {
        return tq::Value(obj.cast<int>());
    } else if (py::isinstance<py::float_>(obj)) {
        return tq::Value(obj.cast<double>());
    } else if (py::isinstance<py::str>(obj)) {
        return tq::Value(obj.cast<std::string>());
    } else if (py::isinstance<py::list>(obj)) {
        std::vector<tq::Value> arr;
        py::list lst = obj.cast<py::list>();
        for (auto item : lst) {
            arr.push_back(python_to_value(py::reinterpret_borrow<py::object>(item)));
        }
        return tq::Value(std::move(arr));
    } else if (py::isinstance<py::dict>(obj)) {
        std::map<std::string, tq::Value> map;
        py::dict dct = obj.cast<py::dict>();
        for (auto item : dct) {
            std::string key = py::str(item.first).cast<std::string>();
            map[key] = python_to_value(py::reinterpret_borrow<py::object>(item.second));
        }
        return tq::Value(std::move(map));
    } else {
        throw std::runtime_error("Unsupported Python type for conversion to Value");
    }
}

// Convert TQ Value to Python object
py::object value_to_python(const tq::Value& val) {
    if (val.is_null()) {
        return py::none();
    } else if (val.is_boolean()) {
        return py::bool_(val.as_boolean());
    } else if (val.is_number()) {
        double num = val.as_number();
        // Return int if it's a whole number
        if (num == static_cast<long long>(num)) {
            return py::int_(static_cast<long long>(num));
        }
        return py::float_(num);
    } else if (val.is_string()) {
        return py::str(val.as_string());
    } else if (val.is_array()) {
        py::list lst;
        for (const auto& elem : val.as_array()) {
            lst.append(value_to_python(elem));
        }
        return lst;
    } else if (val.is_object()) {
        py::dict dct;
        for (const auto& [key, value] : val.as_object()) {
            dct[py::str(key)] = value_to_python(value);
        }
        return dct;
    }
    return py::none();
}

// Query function accepting Python objects
py::list query_python(const std::string& expression, py::object data) {
    // Convert Python data to Value
    tq::Value data_value = python_to_value(data);
    
    // Execute query
    std::vector<tq::Value> results = tq::query_values(expression, data_value);
    
    // Convert results back to Python
    py::list py_results;
    for (const auto& result : results) {
        py_results.append(value_to_python(result));
    }
    
    return py_results;
}

// Query function accepting JSON string
py::list query_json(const std::string& expression, const std::string& json_data) {
    auto results = tq::query(expression, json_data);
    
    py::list py_results;
    for (const auto& result : results) {
        py_results.append(py::str(result));
    }
    
    return py_results;
}

PYBIND11_MODULE(pytq, m) {
    m.doc() = "Python bindings for TQ query engine";
    
    m.def("query", &query_python, 
          py::arg("expression"), 
          py::arg("data"),
          "Query Python data structures with TQ expressions\n\n"
          "Args:\n"
          "    expression (str): TQ query expression (e.g., '.users[].email')\n"
          "    data: Python dict, list, or primitive value\n\n"
          "Returns:\n"
          "    list: Query results as Python objects");
    
    m.def("query_json", &query_json,
          py::arg("expression"),
          py::arg("json_data"),
          "Query JSON data with TQ expressions\n\n"
          "Args:\n"
          "    expression (str): TQ query expression\n"
          "    json_data (str): JSON string\n\n"
          "Returns:\n"
          "    list: Query results as JSON strings");
    
    m.attr("__version__") = "1.0.0";
}
