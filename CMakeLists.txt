cmake_minimum_required(VERSION 3.10.0)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# 在 CMakeLists.txt 中添加


# 编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()
# if(WIN32)
# include_directories("X:/Library/SDL2/include")
# link_directories("X:/Library/SDL2/lib/x64")
# elseif(APPlE)
# include_directories("/Library/SDL2/include")
# link_directories("/Library/SDL2/lib")
# elseif(UNIX)
# include_directories("/usr/local/include")
# link_directories("/usr/local/lib")
# endif()
# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 优先从环境变量中找
# set(SDL2_DIR "X:/Library/SDL2/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${TARGET} 
src/main.cpp
src/game.cpp
src/sceneMain.cpp
src/sceneTitle.cpp)

# 设置项目根目录宏
target_compile_definitions(${TARGET} PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_link_libraries(${TARGET} 
${SDL2_LIBRARIES}
SDL2_image::SDL2_image
SDL2_mixer::SDL2_mixer
SDL2_ttf::SDL2_ttf)

if(WIN32)
    get_target_property(SDL2_DLL SDL2::SDL2 IMPORTED_LOCATION)
    if(SDL2_DLL)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DLL}"
            $<TARGET_FILE_DIR:${TARGET}>
        )
    endif()
    
    # 如果有SDL2main的DLL也需要复制
    get_target_property(SDL2MAIN_DLL SDL2::SDL2main IMPORTED_LOCATION)
    if(SDL2MAIN_DLL)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2MAIN_DLL}"
            $<TARGET_FILE_DIR:${TARGET}>
        )
    endif()
endif()

