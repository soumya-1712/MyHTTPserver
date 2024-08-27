cmake_minimum_required(VERSION 3.3)

project(myhttpserver)  

find_package(OPENGL REQUIRED)

set(FILES
    filesystemrequesthandler.cpp
    filesystemrequesthandler.h
    helprequesthandler.cpp
    helprequesthandler.h
    httpserver.cpp
    httpserver.h
    httprequest.cpp
    httprequest.h
    httpresponse.cpp
    httpresponse.h
    httputility.h
    main.cpp
    mimetypes.cpp
    mimetypes.h
    stringrequesthandler.cpp
    stringrequesthandler.h
   
    )

add_executable(myhttpserver WIN32 ${FILES})  


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
