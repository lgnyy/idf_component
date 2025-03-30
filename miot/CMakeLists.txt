cmake_minimum_required(VERSION 3.5)

file(GLOB MIOT_SRC_FILES *.c)

if(ESP_PLATFORM)
	idf_component_register(SRCS ${MIOT_SRC_FILES}
                       INCLUDE_DIRS "."
					   PRIV_REQUIRES mbedtls json lgnyy__yos)
					   
else()
	add_library(yos STATIC ${MIOT_SRC_FILES})

	target_include_directories(yos PUBLIC .)

	install(TARGETS yos DESTINATION lib)
endif()
