
set(PROVIDER_NAME GKTest)
set(LIBRARY_NAME cmpi${PROVIDER_NAME})
set(MOF GK_Test.mof)

set(provider_SRCS
SettingDB
GK_Test
)

#konkretcmpi_generate(${MOF}
#                     CIM_PROVIDERS
#                     CIM_HEADERS
#)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_library(${LIBRARY_NAME} SHARED
            ${provider_SRCS}
            ${CIM_PROVIDERS}
            ${CIM_HEADERS}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMPI_INCLUDE_DIR}
    
)

#target_link_libraries(${LIBRARY_NAME} ${KONKRETCMPI_LIBRARIES})
target_link_libraries(${LIBRARY_NAME} sqlite3)
target_link_libraries(${LIBRARY_NAME} ind_helper)




# Create registration file
#cim_registration(${PROVIDER_NAME} ${LIBRARY_NAME} ${MOF} share/openlmi-providers)

#install(TARGETS ${LIBRARY_NAME} DESTINATION /usr/lib${LIB_SUFFIX}/cmpi/)
