cmake_minimum_required(VERSION 2.8)

project(plotter)

if(APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lTreePlayer")

# Only necessary on MacOS X to silence a warning:
set( CMAKE_MACOSX_RPATH ON )

# Build the executables:
add_executable(Executable_Plotter app/Plotter.cxx)
target_link_libraries(Executable_Plotter Library_Plotter)

add_executable(Executable_basicPlotter app/basicPlotter.cxx)
target_link_libraries(Executable_basicPlotter Library_Plotter)

find_package(ROOT REQUIRED COMPONENTS Graf Hist Gpad Matrix MathCore RIO Net Tree Minuit)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
if(DEFINED ENV{ROOT_USE_FILE})
  include(${ROOT_USE_FILE})
endif() 

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})


#Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# Build the library:
add_library(   
Library_Plotter Plotter/ReadFiles.h Plotter/HistoPlotter.h Plotter/Trigger.h Plotter/Statistics.h  Plotter/GraphicsOptions.h util/Debug.h util/colormod.h 

Root/ReadFiles.cxx Root/HistoPlotter.cxx Root/Trigger.cxx Root/Statistics.cxx
)


#link ROOT-library ---> MyExecutable
target_include_directories(Executable_Plotter PRIVATE ${ROOT_INCLUDE_DIRS} )
target_link_libraries(Executable_Plotter ${ROOT_LIBRARIES})

target_include_directories(Executable_basicPlotter PRIVATE ${ROOT_INCLUDE_DIRS} )
target_link_libraries(Executable_basicPlotter ${ROOT_LIBRARIES})

#link ROOT-library ---> MyLibrary
target_include_directories(Library_Plotter PRIVATE ${ROOT_INCLUDE_DIRS} )
target_link_libraries(Library_Plotter ${ROOT_LIBRARIES})

