cmake_minimum_required(VERSION 3.0.0)
project(Example VERSION 0.1.0)

# Find and include MPI
# You need to specify CC and CXX to point to MPI compilers
find_package(MPI)
if(MPI_FOUND)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} mpic++ --showme:incdirs
                  OUTPUT_VARIABLE MPI_INCLUDE_PATH)
    string (REPLACE " " ";" MPI_INCLUDE_PATH "${MPI_INCLUDE_PATH}")
  message("MPI_INCLUDE_PATH = ${MPI_INCLUDE_PATH}")
  include_directories(${MPI_INCLUDE_PATH})
else()
  MESSAGE(FATAL_ERROR "We did not find any OpenMPI installation.")
endif()

# Find and include ROOT
find_package(ROOT)
if(ROOT_FOUND)
  message("-- ROOT_VERSION = ${ROOT_VERSION}")
  include(${ROOT_USE_FILE})
endif()

# Generate dictionary
REFLEX_GENERATE_DICTIONARY(myprog-dict util.h SELECTION selection.xml)

add_executable(myprog main.cpp myprog-dict.cxx)

# Link against ROOT libraries
target_link_libraries(myprog ${ROOT_LIBRARIES})
