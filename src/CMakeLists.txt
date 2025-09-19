
add_library(throwinfo SHARED throwinfo/throwinfo.cpp)
set_target_properties(throwinfo PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(throwinfo PRIVATE stdc++exp)

add_executable(testprog tests/testprog.cpp)

#add_executable(extended_exception extended_exception.cpp)
#target_link_libraries(extended_exception PRIVATE stdc++exp)
