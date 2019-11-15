ADD_LIBRARY(extend_cfg global.h CFGFrontendAction.h ASTConsumerForCFG.h ASTVisitorForCFG.h ASTVisitorForCFG.cpp CFGList.h FunctionCFG.cpp FunctionCFG.h)
TARGET_LINK_LIBRARIES(extend_cfg)
SET_TARGET_PROPERTIES(extend_cfg PROPERTIES CXX_VISIBILITY_PRESET hidden)
