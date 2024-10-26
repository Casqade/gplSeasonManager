cmake_minimum_required(VERSION 3.15)

set(TARGET gplSeasonManager)
project(${TARGET} VERSION 1.0 LANGUAGES CXX)


add_executable(${TARGET}
  main.cpp
)

include(GNUInstallDirs)

target_include_directories(${TARGET} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${TARGET} PRIVATE
  -static
)
