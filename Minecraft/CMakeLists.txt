# CMakeList.txt : CMake project for the Application, include source and define
# project specific logic here.
#

file(GLOB_RECURSE SRC
        "src/*.hpp"
        "src/*.cpp"
)

# Add source to this project's executable.
add_executable (Minecraft ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Minecraft PROPERTY CXX_STANDARD 20)
endif()

# Add include directories
target_include_directories(Minecraft PUBLIC ${CMAKE_SOURCE_DIR}/Engine/src)

# Link libraries to the project
target_link_libraries(Minecraft PRIVATE Engine)
