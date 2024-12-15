cmake_minimum_required(VERSION 3.10)
project(AudioWebSocketClient)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS system filesystem)

find_package(nlohmann_json REQUIRED)

find_package(CURL REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(AudioWebSocketClient ${SOURCES})

target_link_libraries(AudioWebSocketClient Boost::system Boost::filesystem nlohmann_json::nlohmann_json CURL::libcurl)

include_directories(${Boost_INCLUDE_DIRS} src)

add_custom_command(TARGET AudioWebSocketClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/scripts
        ${CMAKE_BINARY_DIR}/scripts
)
