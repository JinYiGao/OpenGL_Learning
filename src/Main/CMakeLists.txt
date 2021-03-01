#打包文件 以一个变量名表示
file(GLOB SOURCES *.cpp *.h)
file(GLOB UI_FILES *.ui "../*.qrc") 
#将u文件和生成文件整理在一个文件夹中，非必须
SOURCE_GROUP("Main" FILES ${UI_FILES} ${SOURCES} ${WRAP_FILES} )

#生成库
#add_library(hello_triangle ${SOURCES})
#将源文件资源编译为可执行文件
add_executable(main ${SOURCES} ${UI_FILES} )

#添加程序所需要的的链接库
target_link_libraries(main Qt5::Core Qt5::Widgets Qt5::Gui GLwidgets)