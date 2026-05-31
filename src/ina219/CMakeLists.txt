add_library(ina219 STATIC)

target_sources(ina219 PRIVATE ina219.c)

target_include_directories(ina219 PUBLIC .)