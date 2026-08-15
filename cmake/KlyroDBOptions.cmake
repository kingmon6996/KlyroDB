# CMake Options for KlyroDB

option(KLYRO_BUILD_TESTS "Build KlyroDB tests" ON)
option(KLYRO_BUILD_EXAMPLES "Build KlyroDB examples" ON)
option(KLYRO_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(KLYRO_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

# Function to add strict compiler flags to a target
function(klyro_enable_strict_warnings target_name)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Woverloaded-virtual
            -Wnull-dereference
            -Werror
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target_name} PRIVATE
            /W4
            /WX
            /permissive-
        )
    endif()
endfunction()

# Function to add sanitizers
function(klyro_enable_sanitizers target_name)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        if(KLYRO_ENABLE_ASAN)
            target_compile_options(${target_name} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
            target_link_options(${target_name} PRIVATE -fsanitize=address)
        endif()
        if(KLYRO_ENABLE_UBSAN)
            target_compile_options(${target_name} PRIVATE -fsanitize=undefined)
            target_link_options(${target_name} PRIVATE -fsanitize=undefined)
        endif()
    endif()
endfunction()
