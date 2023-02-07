cmake_minimum_required(VERSION 3.3)
project(Vulcano C)

set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")
set(CMAKE_C_FLAGS_MINSIZEREL "-Wall -Wextra -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Wall -Wextra -g -O2")
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -O2")

find_package(Vulkan REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} "include/")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.c)

set (EXCLUDE_DIR "/CMakeFiles/")
file (GLOB_RECURSE SRC_FILES "*.cpp" "*.c")
foreach (TMP_PATH ${SRC_FILES})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM SRC_FILES ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

add_executable(vulcano ${SRC_FILES})
target_link_libraries(vulcano glfw vulkan ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})
