cmake_minimum_required(VERSION 3.15)
project(fly_plane)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找SFML库
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 设置包含目录
include_directories(${CMAKE_SOURCE_DIR})

# 添加可执行文件
add_executable(${PROJECT_NAME}
    main.cpp
    Player.cpp
    Bullet.cpp
    Shooter.cpp
    Enemy.cpp
    Laser.cpp
)

# 链接SFML库
target_link_libraries(${PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system
)
