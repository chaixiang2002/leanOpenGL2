cmake_minimum_required(VERSION 3.0.0) # 最小cmake版本

# 项目名
project(demo1 VERSION 0.1.0 LANGUAGES C CXX)

# 如果项目2进制目录 == 项目源码目录，报错
if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(FATAL_ERROR "The binary directory of CMake cannot be the same as source directory! "
        "Do not use 'cmake .'! Please consider use 'cmake -B build' to specify a different binary directory. "
        "Otherwise this project may fail to build, or make Git hard to exclude binary files. "
        "For Windows, it is highly recommended to use Visual Studio as IDE for CMake projects.")
endif()

# 打印构建类型
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

# 如果不是MSVC
if (NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)  # Compilation speed up: https://ccache.dev
    if (CCACHE_PROGRAM)
        message(STATUS "Found CCache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()
endif()

# 如果是MSVC
if (MSVC)  # 1. fix string literial cannot include unicode characters; 2. rid min and max macros; 3. get back M_PI, M_E macros
    add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/utf-8$<SEMICOLON>/DNOMINMAX$<SEMICOLON>/D_USE_MATH_DEFINES>)
    add_compile_options($<$<COMPILE_LANG_AND_ID:C,MSVC>:/utf-8$<SEMICOLON>/DNOMINMAX$<SEMICOLON>/D_USE_MATH_DEFINES>)
elseif (CMAKE_COMPILER_IS_GNUCC)  # Add necessary checks and warnings
    if (CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
        add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wall$<SEMICOLON>-Wextra>)
        add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-ftrapv$<SEMICOLON>-fbounds-check$<SEMICOLON>-fsanitize-trap>)
        # add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-fsanitize=address$<SEMICOLON>-fsanitize=leak>)
        # link_libraries(asan)
    endif()
endif()

# 项目目录加入cmake
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/*.h include/*.hpp)
add_executable(${PROJECT_NAME} ${sources} ${headers})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# 加入3个库
add_subdirectory(glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
add_subdirectory(glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

# 结构
target_compile_definitions(${PROJECT_NAME} PUBLIC -DOPENGLTUTOR_HOME=\"${CMAKE_CURRENT_SOURCE_DIR}/\")



# include(CTest)
# enable_testing()

# add_executable(demo1 main.cpp)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
