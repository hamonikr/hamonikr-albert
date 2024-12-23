project(websearch VERSION 8.1)

albert_plugin(
    SOURCE_FILES
        ${PROJECT_NAME}.qrc
        src/*.cpp
        src/*.h
        src/*.ui
)

# 리소스 파일 설치
install(
    DIRECTORY resources
    DESTINATION ${CMAKE_INSTALL_DATADIR}/albert/plugins/${PROJECT_NAME}
    FILES_MATCHING 
        PATTERN "*.svg"
        PATTERN "*.png"
        PATTERN "*.json"
)
