find_package(OpenCV REQUIRED)
# find_package(Tesseract REQUIRED)
include_directories(/usr/include/tesseract ${OpenCV_INCLUDE_DIRS} )

add_library(reader
    imageprocessor.cpp include/imageprocessor.h
    sudokureader.cpp include/sudokureader.h
)


target_include_directories(reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
