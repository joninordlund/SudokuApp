add_library(solver
    DLXBuilder.cpp
    sudokusolver.cpp
)

target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
