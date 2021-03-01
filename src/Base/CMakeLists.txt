file(GLOB SOURCES "*.cpp" "*.h")
SOURCE_GROUP("Sources" FILES ${SOURCES} )
add_library(base ${SOURCES})
target_link_libraries(base Qt5::Core Qt5::Widgets Qt5::Gui)