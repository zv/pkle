cmake_minimum_required(VERSION 3.0)

# where is boost?
include_directories(/opt/boost)
link_directories(/opt/boost/stage/lib)

# where is z3?
include_directories(./z3/src/api)
include_directories(./z3/src/api/c++)
link_directories(./z3/build)

include_directories(.)

#set(LibSources
#  node.cpp
#  node_mem.cpp
#  s2.cpp
#)

#add_library(pkle ${LibSources})

set(BinSources
  main.cpp
  context.cpp
)

set(BinLibaries
  z3
)

add_executable(pkle ${BinSources})
target_link_libraries(pkle ${BinLibaries})

#set(TestSources
#  tests.cpp
#)

#include_directories(/opt/googletest/googletest/include)
#link_directories(/opt/googletest/googlemock/gtest)

#set(TestLibs
#  belprop
#  gtest
#  pthread
#)

#add_executable(tests ${TestSources})
#target_link_libraries(tests ${TestLibs})
