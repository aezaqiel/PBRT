cmake_minimum_required(VERSION 3.10)

project(PBRT C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(thirdparty/glm)

file(TO_CMAKE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/render OUTPUT_FOLDER)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/PathConfig.inl
    ${CMAKE_CURRENT_BINARY_DIR}/generated/PathConfig.inl
    @ONLY
)

add_executable(${PROJECT_NAME}
    src/Main.cpp

    src/Core/Types.hpp
    src/Core/Rng.hpp

    src/Containers/Ray.hpp
    src/Containers/Interval.hpp
    src/Containers/Interval.cpp
    src/Containers/AABB.hpp
    src/Containers/AABB.cpp

    src/Hittables/Hittable.hpp
    src/Hittables/Sphere.hpp
    src/Hittables/Sphere.cpp

    src/Materials/Material.hpp
    src/Materials/Lambertian.hpp
    src/Materials/Lambertian.cpp
    src/Materials/Metal.hpp
    src/Materials/Metal.cpp
    src/Materials/Dielectric.hpp
    src/Materials/Dielectric.cpp

    src/Scene/Camera.hpp
    src/Scene/Camera.cpp
    src/Scene/BVHNode.hpp
    src/Scene/Scene.hpp
    src/Scene/Scene.cpp

    src/Platform/STBImpl.cpp
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    src
    thirdparty/stb
    ${CMAKE_CURRENT_BINARY_DIR}/generated
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
