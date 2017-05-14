cmake_minimum_required(VERSION 3.3)
project(ChatServer)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
set(SOURCE_FILES main.c main.c MessageStructure/messagetype.h MessageStructure/limits.h MessageStructure/user.h MessageStructure/user.c DataStructure/onlinelinklist.c DataStructure/onlinelinklist.h Socket/server.c Socket/server.h Socket/socketmarco.h)
add_executable(ChatServer ${SOURCE_FILES})
find_package(Threads REQUIRED)
target_link_libraries(ChatServer Threads::Threads)