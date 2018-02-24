set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core       5.3 REQUIRED)
find_package(Qt5Gui        5.3 REQUIRED)
find_package(Qt5Widgets    5.3 REQUIRED)

set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/Test)

set(SRC_LIST
    ${SRCROOT}/main.cpp
    ${SRCROOT}/mainwindow.cpp
    ${SRCROOT}/mainwindow.h
)

set(UIS
    ${SRCROOT}/mainwindow.ui
)

set(RES
    ${SRCROOT}/res.qrc
)

add_executable(Test
    ${SRC_LIST}
    ${UIS}
    ${RES}
)

target_link_libraries(Test
    TabToolbar
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

set_target_properties(Test PROPERTIES DEBUG_POSTFIX d)
set_target_properties(Test PROPERTIES FOLDER "Examples")

install(TARGETS Test
        RUNTIME DESTINATION ${INSTALL_MISC_DIR}/examples/Test COMPONENT examples
        BUNDLE DESTINATION ${INSTALL_MISC_DIR}/examples/Test COMPONENT examples)
        
install(FILES ${SRC_LIST} ${UIS} ${RES}
        DESTINATION ${INSTALL_MISC_DIR}/examples/Test
        COMPONENT examples)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/examples/Test/icons"
        DESTINATION ${INSTALL_MISC_DIR}/examples/Test
        COMPONENT examples)
