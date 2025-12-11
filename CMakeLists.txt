cmake_minimum_required(VERSION 3.10)

project(PBRT C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(thirdparty/glm)

add_executable(${PROJECT_NAME}
    src/Main.cpp
    src/Types.hpp

    src/Camera.hpp
    src/Camera.cpp

    src/Containers/Ray.hpp
    src/Containers/Interval.hpp
    src/Containers/Interval.cpp

    src/Geometry/Hittable.hpp
    src/Geometry/HittableList.hpp
    src/Geometry/HittableList.cpp
    src/Geometry/Sphere.hpp
    src/Geometry/Sphere.cpp

    src/Platform/STBImpl.cpp
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    src
    thirdparty/stb
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glm
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    NOMINMAX
    GLM_ENABLE_EXPERIMENTAL
)

target_precompile_headers(${PROJECT_NAME}
PRIVATE
    src/PCH.hpp
)
