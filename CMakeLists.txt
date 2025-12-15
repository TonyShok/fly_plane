cmake_minimum_required(VERSION 3.15)
project(fly_plane)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找SFML库
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 设置包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    code/main.cpp
    code/Player.cpp
    code/Bullet.cpp
    code/Shooter.cpp
    code/Enemy.cpp
    code/Laser.cpp
)

# 链接SFML库
target_link_libraries(${PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system
)
