project(imgui)

file(GLOB_RECURSE SRC src/*.cpp)

add_library(imgui STATIC ${SRC})
target_include_directories(imgui PRIVATE ./include/ImGui)

#link glfw
target_include_directories(imgui PRIVATE ../glfw/include)
target_link_libraries(imgui PRIVATE glfw)

#link opengl
if(WIN32)
    target_link_libraries(imgui PRIVATE opengl32.lib)
elseif(UNIX)
    target_link_libraries(imgui PRIVATE libGL.so)
else()
    message(FATAL_ERROR "Don't support this platform!")
endif()