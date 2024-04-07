cmake_minimum_required(VERSION 3.0.0)
project(opencv VERSION 0.1.0 LANGUAGES C CXX)

set(OpenCV_DIR /opt/homebrew/Cellar/opencv/4.8.1_5)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS})

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")  

# set(DIR_SRCS1
#     ./p1/main.cpp
#     ./p1/ImageTransform.cpp
#     ./p1/RWImage.cpp
# )

# set(DIR_SRCS2
#     ./p2/main.cpp
#     ./p2/RWImage.cpp
#     ./p2/HomographicTrans.cpp
# )

set(DIR_SRCS3
    ./p3/main.cpp
    ./p3/MorphologyProc.cpp
    ./p3/RWImage.cpp
)

# add_executable(p1_executable ${DIR_SRCS1})
# # target_link_libraries(p1_executable PRIVATE ${OpenCV_LIBS})

# add_executable(p2_executable ${DIR_SRCS2})
# target_link_libraries(p2_executable PRIVATE ${OpenCV_LIBS})

add_executable(p3_executable ${DIR_SRCS3})
target_link_libraries(p3_executable PRIVATE ${OpenCV_LIBS})
