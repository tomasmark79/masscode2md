cmake_minimum_required(VERSION 3.5.0)
project(masscode2md VERSION 0.1.0 LANGUAGES C CXX)

set(LOHHM_HPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/json/single_include/nlohmann)

add_executable(masscode2md main.cpp) #${LOHHM_HPP_DIR}/json.hpp

target_include_directories(masscode2md PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${LOHHM_HPP_DIR}/
)