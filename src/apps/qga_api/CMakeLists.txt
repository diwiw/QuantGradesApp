add_executable(quantgrades
    main.cpp
)

target_include_directories(quantgrades
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(quantgrades
    PRIVATE
        qga_api_lib
        qga_core
        qga_utils
        fmt::fmt
)
