set(SRC
    main.cpp
    monitor.cpp
)

add_executable(mdnsdebug ${SRC})
set_target_properties(mdnsdebug PROPERTIES CXX_STANDARD 11)

target_link_libraries(mdnsdebug qmdnsengine)

install(TARGETS mdnsdebug RUNTIME DESTINATION bin)
