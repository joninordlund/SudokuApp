find_package(OpenCV REQUIRED)
# find_package(Tesseract REQUIRED)
include_directories(/usr/include/tesseract ${OpenCV_INCLUDE_DIRS} )

add_library(reader
    imageprocessor.cpp
    sudokureader.cpp
)


target_include_directories(reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
