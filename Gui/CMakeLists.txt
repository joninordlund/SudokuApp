add_executable(SudokuApp
    main.cpp
    cell.cpp
    grid.cpp
    util.cpp
)

target_include_directories(SudokuApp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SudokuApp
    PRIVATE
        Qt6::Widgets
        solver
        reader
)
