cmake_minimum_required(VERSION 3.0)

# where is boost?
include_directories(/opt/boost)
link_directories(/opt/boost/stage/lib)

# where is z3?
include_directories(./z3/src/api)
include_directories(./z3/src/api/c++)
link_directories(./z3/build)

include_directories(.)

set(LibSources
  context.cpp
)

add_library(pklez3 ${LibSources})

set(BinSources
  main.cpp
)

set(BinLibaries
  pklez3
  z3
)

add_executable(pkle ${BinSources})
target_link_libraries(pkle ${BinLibaries})

set(TestSources
  tests.cpp
)

include_directories(/opt/googletest/googletest/include)
link_directories(/opt/googletest/googlemock/gtest)

set(TestLibs
  pklez3
  z3
  gtest
  pthread
)

add_executable(tests ${TestSources})
target_link_libraries(tests ${TestLibs})
