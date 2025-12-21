add_executable(SudokuApp
    main.cpp
    cell.cpp include/cell.h
    grid.cpp include/grid.h
    include/mainwindow.h mainwindow.cpp
    include/toggleswitch.h toggleswitch.cpp
    include/sudokuboard.h sudokuboard.cpp
    include/sudokuimage.h sudokuimage.cpp
    include/history.h history.cpp
    include/cellchange.h
    resources.qrc
    include/sudokuserializer.h sudokuserializer.cpp
    include/countlabel.h countlabel.cpp
    style.qss


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
