set(HEADERS src/detail/server.hpp 
            src/detail/handler.hpp)

set(SOURCES src/detail/server.cpp 
            src/detail/handler.cpp 
            src/main.cpp)

add_executable(${PROGRAM2_TARGET} ${HEADERS} ${SOURCES})

target_include_directories(${PROGRAM2_TARGET} PRIVATE ${PROJECT_ROOT_DIR})