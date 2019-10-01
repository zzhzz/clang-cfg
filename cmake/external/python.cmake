
MESSAGE(STATUS "PY_VERSION = ${PY_VERSION}")
FIND_PACKAGE(PythonInterp ${PY_VERSION} EXACT)
FIND_PACKAGE(PythonLibs ${PY_VERSION} EXACT)

ADD_LIBRARY(python STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET python PROPERTY IMPORTED_LOCATION ${PYTHON_LIBRARIES})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR})


