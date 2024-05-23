include(FetchContent)

FetchContent_Declare(
    bme68x_driver
    GIT_REPOSITORY https://github.com/boschsensortec/BME68x_SensorAPI.git
    GIT_TAG v4.4.8
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/driver/bme68x/src
)

FetchContent_MakeAvailable(bme68x_driver)