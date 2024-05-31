set(requires "")
idf_build_get_property(target IDF_TARGET)

if(${target} STREQUAL "linux")
    list(APPEND requires esp_stubs esp-tls esp_http_server protocol_examples_common nvs_flash driver)
endif()
idf_component_register(SRCS "secret.c" "switch.c" "motor.c" "server.c" "main.c"
                    INCLUDE_DIRS "include"
                    REQUIRES ${requires})
