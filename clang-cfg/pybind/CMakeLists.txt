ADD_SUBDIRECTORY(extend_cfg)
ADD_SUBDIRECTORY(ast)
ADD_SUBDIRECTORY(data_struct)

ADD_LIBRARY(ParseHelper ParseHelper.cpp ParseHelper.h)

PYTHON_ADD_MODULE(clang_cfg pybind.cpp)

TARGET_LINK_LIBRARIES(clang_cfg extend_cfg ast CFG ParseHelper)

TARGET_LINK_LIBRARIES(clang_cfg ${CLANG_LIBS} ${LLVM_LIBS})

IF(APPLE)
    LIST(APPEND SYSTEM_LIBS pthread dl z curses m xml2)
ELSEIF(UNIX)
    LIST(APPEND SYSTEM_LIBS pthread dl tinfo z)
ENDIF()

TARGET_LINK_LIBRARIES(clang_cfg ${SYSTEM_LIBS})
SET_TARGET_PROPERTIES(clang_cfg PROPERTIES CXX_VISIBILITY_PRESET hidden)




