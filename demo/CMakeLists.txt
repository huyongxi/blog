


aux_source_directory(. DEMO_SRC_LIST)

add_executable(demo ${DEMO_SRC_LIST})

target_include_directories(demo 
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(demo blog)