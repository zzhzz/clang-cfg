ADD_SUBDIRECTORY(data_struct)

ADD_LIBRARY(clang_tool global.h CFGFrontendAction.h ASTConsumerForCFG.h ASTVisitorForCFG.h ASTVisitorForCFG.cpp CFGList.h ParseHelper.cpp ParseHelper.h FunctionCFG.cpp FunctionCFG.h)

TARGET_LINK_LIBRARIES(clang_tool CFG)
SET_TARGET_PROPERTIES(clang_tool PROPERTIES CXX_VISIBILITY_PRESET hidden)
