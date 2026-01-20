add_library(qga_api_lib STATIC
    ApiServer.cpp
)

target_include_directories(qga_api_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}        
)



target_link_libraries(qga_api_lib
    PUBLIC
        qga_core
        qga_utils
        fmt::fmt
        spdlog::spdlog
        httplib::httplib
)
