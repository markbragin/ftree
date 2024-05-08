cmake_minimum_required(VERSION 3.10)

project(ftree)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	src/main.c
	src/dynamic_array.c
    src/tree.c
	src/argparser.c
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -D_DEFAULT_SOURCE
)

target_include_directories(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCE_DIR}/src)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
