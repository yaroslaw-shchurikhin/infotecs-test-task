set(HEADERS src/detail/buffer.hpp 
            src/detail/handler.hpp 
            src/supply/support_funcs.hpp
            src/ui/user_interface.hpp)

set(SOURCES src/detail/handler.cpp 
            src/supply/support_funcs.cpp
            src/ui/user_interface.cpp
            src/main.cpp)

add_executable(${PROGRAM1_TARGET} ${HEADERS} ${SOURCES})

target_include_directories(${PROGRAM1_TARGET} PRIVATE ${PROJECT_ROOT_DIR})