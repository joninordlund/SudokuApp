qt_standard_project_setup()

qt_add_executable(sudokuapp
    main.cpp
    # lisää myöhemmin .cpp/.h/.ui tiedostoja
)

target_link_libraries(sudokuapp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    sudokusolver
    sudokureader
)
