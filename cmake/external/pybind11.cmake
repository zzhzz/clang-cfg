INCLUDE(ExternalProject)

SET(PYBIND_SOURCE_DIR ${THIRD_PARTY_PATH}/pybind11)
INCLUDE_DIRECTORIES(${PYBIND_SOURCE_DIR}/src/extern_pybind/include)

ExternalProject_Add(
        extern_pybind
        GIT_REPOSITORY "https://github.com/pybind/pybind11.git"
        GIT_TAG "v2.2.4"
        PREFIX ${PYBIND_SOURCE_DIR}
)

ADD_LIBRARY(pybind INTERFACE)
ADD_DEPENDENCIES(pybind extern_pybind)