set (LOGGER_SOURCES 
        logger.c
        logger.h
        loggerconf.c
        loggerconf.h)

add_library(Logger SHARED
                ${LOGGER_SOURCES})

set_target_properties(Logger
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ../release/lib
    ARCHIVE_OUTPUT_DIRECTORY ../release/lib
)                
