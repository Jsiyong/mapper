cmake_minimum_required(VERSION 3.13)
project(mapper)

## 初始设置 #############################
set(CMAKE_CXX_STANDARD 14)
set(BOOST_MIN_VERSION "1.72.0")
set(BOOST_ROOT "/usr/local/include/boost")
message("project dir: " ${CMAKE_SOURCE_DIR})
#项目文件路径
set(SOURCE_FILES_DIR ${CMAKE_SOURCE_DIR}/src)

## 一些定义 #############################
# 定义递归查找头文集路径的汉书
function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir}) # 当前路径是一个目录吗，是的话就加入到包含目录
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif ()
    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach (sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub}) # 对子目录递归调用，包含
        endif ()
    endforeach ()
endfunction()

## Boost设置 #############################
#添加头文件搜索路径
include_directories(/usr/local/include /usr/local/mysql/include)
#添加库文件搜索路径
link_directories(/usr/local/lib /usr/local/mysql/lib)

# 相关模块导入
find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS log REQUIRED)

## 项目最后设置 ############################
# 先添加可执行文件路径
include_sub_directories_recursively(${SOURCE_FILES_DIR})

# 添加所有的.h/.cpp/.hpp文件
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_FILES_DIR}/*.cpp ${SOURCE_FILES_DIR}/*.h ${SOURCE_FILES_DIR}/*.hpp)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
# 链接库文件
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} libmysqlclient.so)