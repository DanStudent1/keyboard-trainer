cmake_minimum_required(VERSION 3.15)
project(trpp_project)

set(CMAKE_CXX_STANDARD 17)

# Указываем путь к vcpkg, если переменная окружения не установлена
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/Users/User/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# Ищем SFML с нужными компонентами
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Создаём исполняемый файл
add_executable(trainer main.cpp)

# Линкуем библиотеки SFML
target_link_libraries(trainer PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
