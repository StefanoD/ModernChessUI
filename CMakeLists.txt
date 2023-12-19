cmake_minimum_required(VERSION 3.16)

project(ModernChessUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appModernChessUI
    main.cpp
)

qt_add_qml_module(appModernChessUI
    URI ModernChessUI
    VERSION 1.0
    QML_FILES Main.qml Board.qml
    SOURCES BoardFactory.h BoardFactory.cpp
    SOURCES SquareModel.h SquareModel.cpp
    SOURCES Board.h Board.cpp
)

qt_add_resources(appModernChessUI "figures"
    PREFIX "/"
    FILES
        resources/white-king.svg
        resources/white-bishop.svg
        resources/white-knight.svg
        resources/white-pawn.svg
        resources/white-queen.svg
        resources/white-rook.svg
        resources/black-king.svg
        resources/black-knight.svg
        resources/black-pawn.svg
        resources/black-bishop.svg
        resources/black-pawn.svg
        resources/black-queen.svg
        resources/black-rook.svg
)

add_subdirectory(ModernChessLib)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appModernChessUI PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appModernChessUI
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appModernChessUI
    PRIVATE
    Qt6::Quick
    modern-chess-lib
)

include(GNUInstallDirs)
install(TARGETS appModernChessUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
