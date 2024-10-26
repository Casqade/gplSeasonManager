cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

set(TARGET gplSeasonManager)
project(${TARGET} VERSION 1.0 LANGUAGES CXX)


option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_executable(${TARGET}
  main.cpp
)

set_target_properties(${TARGET} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_STANDARD 20
)

if(${BUILD_SHARED_LIBS})
  set_target_properties(${TARGET} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
  )
else()
  set_target_properties(${TARGET} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
  )

  if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(${TARGET} PRIVATE
      -static
    )
  endif()
endif()


include(GNUInstallDirs)

target_include_directories(${TARGET} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
)
