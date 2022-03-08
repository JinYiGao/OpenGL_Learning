file(GLOB SOURCES "*.cpp" "*.h")
SOURCE_GROUP("Sources" FILES ${SOURCES} )
add_library(GLwidgets ${SOURCES})
target_link_libraries(GLwidgets Qt5::Core Qt5::Widgets Qt5::Gui base assimp::assimp)