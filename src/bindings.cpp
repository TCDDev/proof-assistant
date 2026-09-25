#include <pybind11/pybind11.h>

#include <proof/runtime.hpp>

namespace py = pybind11;

PYBIND11_MODULE(_proof, m) {
    py::class_<Proof::Proposition>(m, "Proposition")
    .def(py::init<std::string>())
    .def_readonly("name", &Proof::Proposition::name);
}