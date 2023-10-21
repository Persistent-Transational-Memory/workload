project(workload_test)

FILE(GLOB_RECURSE header_files CONFIGURE_DEPENDS ./*.hpp ./*.h)
FILE(GLOB_RECURSE source_files CONFIGURE_DEPENDS ./*.cpp)

message(STATUS "${PROJECT_NAME} Configuration")
message(STATUS "---- Header Files:")
    foreach(header ${header_files})
        message(STATUS "----\t ${header}")
    endforeach()
message(STATUS "---- Source Files:")
    foreach(source ${source_files})
        message(STATUS "----\t ${source}")
    endforeach()

add_executable(${PROJECT_NAME} ${header_files} ${source_files})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        gtest
        glog
        hwloc
        transaction_manager
        storage_manager
        index
        concurrent_control
        workload
        util)