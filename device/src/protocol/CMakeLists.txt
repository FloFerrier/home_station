add_library(protocol STATIC)

target_sources(protocol PRIVATE protocol.c)

target_include_directories(protocol PUBLIC .)