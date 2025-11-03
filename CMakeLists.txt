cmake_minimum_required(VERSION 3.10)

project(GLCraft)
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.h" "src/*.hpp")

add_subdirectory(3rdparty)
add_executable(GLCraft ${SRC_FILES})

target_link_libraries(GLCraft PRIVATE	glad
										glfw
										SQLiteCpp
										imgui
)
target_include_directories(GLCraft PRIVATE	"3rdparty/glad/include" 
											"3rdparty/glfw/include"
											"3rdparty/glm"
											"3rdparty/stb_image"
											"3rdparty/FastNoise"
											"3rdparty/SQLiteCpp/include"
											"3rdparty/imgui/include"
)
file(COPY ${PROJECT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})