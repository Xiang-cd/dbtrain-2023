find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser sql.y ${CMAKE_CURRENT_SOURCE_DIR}/sql.tab.cpp
        DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/sql.tab.h)
FLEX_TARGET(scanner sql.l ${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.cpp)

ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_library(sql_parser ${BISON_parser_OUTPUT_SOURCE} ${FLEX_scanner_OUTPUTS})
