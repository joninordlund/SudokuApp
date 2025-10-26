find_package(OpenCV REQUIRED)

add_library(sudoku_reader
    reader.cpp
    reader.h
)

target_include_directories(sudoku_reader PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sudoku_reader PUBLIC ${OpenCV_LIBS})
