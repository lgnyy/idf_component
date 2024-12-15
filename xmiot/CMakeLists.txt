cmake_minimum_required(VERSION 3.5)

file(GLOB XMIOT_SRC_FILES *.c)

if(ESP_PLATFORM)
	idf_component_register(SRCS ${XMIOT_SRC_FILES}
                       INCLUDE_DIRS ".")

else()
	add_library(yos STATIC ${XMIOT_SRC_FILES})

	target_include_directories(yos PUBLIC .)

	install(TARGETS yos DESTINATION lib)
endif()
