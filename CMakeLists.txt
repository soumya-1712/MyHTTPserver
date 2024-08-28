cmake_minimum_required(VERSION 3.3)

project(myhttpserver)  

find_package(OPENGL REQUIRED)

set(FILES
    RequestForFile.cpp
    RequestForFile.h
    Help.cpp
    Help.h
    Server.cpp
    Server.h
    Request.cpp
    Request.h
    Response.cpp
    Response.h
    HttpUtil.h
    main.cpp
    mimetypes.cpp
    mimetypes.h
    RequestForString.cpp
    RequestForString.h
    )

add_executable(myhttpserver WIN32 ${FILES})  

target_include_directories(myhttpserver  
    PRIVATE libs/system.io/include
    PRIVATE libs/imgui
    )

target_compile_features(myhttpserver  
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    PRIVATE cxx_thread_local
    )

target_link_libraries(myhttpserver  
    ws2_32 ${OPENGL_LIBRARIES}
    )

target_compile_definitions(myhttpserver  
    PUBLIC -DWIN32
    )
