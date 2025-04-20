cmake_minimum_required(VERSION 3.5)

file(GLOB YOS_SRC_FILES *.c)

if(ESP_PLATFORM)
	file(GLOB YOS_ESP_SRC_FILES src_esp/*.c)
	idf_component_register(SRCS ${YOS_SRC_FILES} ${YOS_ESP_SRC_FILES}
                       INCLUDE_DIRS "include"
					   REQUIRES nvs_flash esp_wifi esp_event esp_http_client esp_http_server mqtt
					   PRIV_REQUIRES esp-tls)
					   
elseif(WIN32_PLATFORM)
	file(GLOB YOS_WIN32_SRC_FILES src_win32/*.c)
	add_library(yos STATIC ${YOS_SRC_FILES} ${YOS_WIN32_SRC_FILES})

	target_include_directories(yos PUBLIC ./include)

	install(TARGETS yos DESTINATION lib)
endif()
