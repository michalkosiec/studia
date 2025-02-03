# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/kodowanie_huffmana_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/kodowanie_huffmana_autogen.dir/ParseCache.txt"
  "kodowanie_huffmana_autogen"
  )
endif()
