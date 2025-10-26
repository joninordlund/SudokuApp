add_library(sudokusolver
    solver.cpp
    solver.h
)

target_include_directories(solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
