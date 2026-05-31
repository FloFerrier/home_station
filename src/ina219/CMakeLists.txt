add_library(ina219 STATIC)

target_sources(ina219 PRIVATE ina219.c)

#target_link_libraries(led PRIVATE
#    stm32_hal_driver
#)

target_include_directories(ina219 PUBLIC .)