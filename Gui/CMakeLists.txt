add_executable(SudokuApp
    main.cpp
    cell.cpp include/cell.h
    grid.cpp include/grid.h
    include/mainwindow.h mainwindow.cpp
    include/toggleswitch.h toggleswitch.cpp
    include/sudokumatrix.h sudokumatrix.cpp
)

target_include_directories(SudokuApp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SudokuApp
    PRIVATE
        Qt6::Widgets
        solver
        reader
)
