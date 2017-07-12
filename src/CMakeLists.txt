set(SRC
    main.cpp
    monitor.cpp
)

add_executable(qmdnsdebug ${SRC})
set_target_properties(qmdnsdebug PROPERTIES CXX_STANDARD 11)

target_link_libraries(qmdnsdebug qmdnsengine)

install(TARGETS qmdnsdebug RUNTIME DESTINATION bin)
