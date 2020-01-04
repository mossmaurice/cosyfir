cmake_minimum_required(VERSION 3.13)
project (yaais-node)


# Set CMake compiler
set(CMAKE_CXX_COMPILER      arm-none-eabi-g++)
set(CMAKE_C_COMPILER        arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER      arm-none-eabi-as)

add_executable(yaais-node main.cpp)

# necessary as _exit() is undefined
target_link_libraries(yaais-node PRIVATE -specs=nosys.specs)

target_compile_options(yaais-node PRIVATE -mcpu=cortex-m0)

#-nostartfiles -ffreestanding -specs=nano.specs -fno-exceptions
