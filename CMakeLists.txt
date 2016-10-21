cmake_minimum_required(VERSION 2.8)
project(vulkan_tut)
set(VULKAN_SDK "~/VulkanSDK/1.0.30.0/x86_64/")

include_directories(${VULKAN_SDK}/include)
link_directories(${VULKAN_SDK}/lib)
set (CMAKE_CXX_LINK_EXECUTABLE
"${CMAKE_CXX_LINK_EXECUTABLE} -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp")
set(SOURCE_FILES
	src/main.cpp)

add_executable(vulkan_tut ${SOURCE_FILES})

set_property(TARGET vulkan_tut PROPERTY CXX_STANDARD 11)
target_link_libraries(vulkan_tut glfw3 vulkan)
