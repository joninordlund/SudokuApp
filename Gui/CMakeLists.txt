add_executable(SudokuApp
    main.cpp
    cell.cpp cell.h
    grid.cpp grid.h
    mainwindow.h mainwindow.cpp
    toggleswitch.h toggleswitch.cpp
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
