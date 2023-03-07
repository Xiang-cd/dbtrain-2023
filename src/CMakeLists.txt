add_subdirectory(parser)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS **/*.cpp)
list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/parser/lex.yy.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/parser/sql.tab.cpp")

add_library(thdb STATIC ${sources} result/raw_printer.cpp result/raw_printer.h result/printers.h)

add_executable(cli cli.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(cli thdb sql_parser Threads::Threads)
