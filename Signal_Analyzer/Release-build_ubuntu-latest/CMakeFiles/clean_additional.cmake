# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles/Signal_Analyzer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Signal_Analyzer_autogen.dir/ParseCache.txt"
  "Signal_Analyzer_autogen"
  )
endif()
