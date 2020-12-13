file(GLOB_RECURSE SOURCE *.hpp *.cpp)
add_executable(NesEMU ${SOURCE})

target_include_directories(NesEMU PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
