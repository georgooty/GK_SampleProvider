
# This macro takes name of the MOF for one provider and header files using
# konkretcmpi. It also generates provider skeleton if it doesn't exist.
#
# @param[in] MOF name of the MOF file (should be in mof/ directory of the project root)
# @param[out] CIM_PROVIDERS list of sources of the provider generated from the MOF
# @param[out] CIM_HEADERS list of header file generated from the MOF
#
macro(konkretcmpi_generate MOF CIM_PROVIDERS CIM_HEADERS)
    # Check if MOF exists
    set(MOF_FILE ${CMAKE_SOURCE_DIR}/mof/${MOF})
    message(STATUS "Using mof ${MOF} ${MOF_FILE}")
    if (NOT EXISTS ${MOF_FILE})
        message(FATAL_ERROR "MOF file ${MOF} not found")
    endif (NOT EXISTS ${MOF_FILE})

    # Read CIM classes out of MOF file
    file(READ ${MOF_FILE} MOF_CONTENT)
    string(REGEX MATCHALL "class [A-Za-z0-9_]+" CIM_CLASSESX ${MOF_CONTENT})
    set(CIM_CLASSES "")
    foreach(CLASSX ${CIM_CLASSESX})
        string(REPLACE "class " "" CLASS ${CLASSX})
        set(CIM_CLASSES ${CIM_CLASSES} ${CLASS})
    endforeach(CLASSX ${CIM_CLASSESX})

    list(LENGTH CIM_CLASSES LEN)
    if (${LEN} EQUAL 0)
        message(FATAL_ERROR "No class found in the MOF file ${MOF_FILE}")
    else (${LEN} EQUAL 0)
        # Get headers and sources names from the list of CIM classes
        set(HEADERS "")
        set(PROVIDERS "")
        set(GENERATE_PROVIDERS "")
        set(NEW_PROVIDERS "")
        foreach(CLASS ${CIM_CLASSES})
            # Add generated header to the list
            set(HEADERS ${HEADERS} ${CLASS}.h)
            # Get name of the source file
            set(PROVIDER ${CLASS}Provider.c)
            # If the provider doesn't exist, generate it
            if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PROVIDER})
                # Part of generating command - passed to konkret
                set(GENERATE_PROVIDERS ${GENERATE_PROVIDERS} -s ${CLASS})
                # List of freshly generated providers
                set(NEW_PROVIDERS ${NEW_PROVIDERS} ${PROVIDER})
            endif (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PROVIDER})
            # Add provider source to the list
            set(PROVIDERS ${PROVIDERS} ${PROVIDER})
        endforeach(CLASS ${CIM_CLASSES})

        # Generate headers for CIM classes
        set(ENV{KONKRET_SCHEMA_DIR} "/usr/share/sfcb/CIM")
        execute_process(COMMAND ${KONKRETCMPI_KONKRET}
                                #-m /usr/share/sblim-cmpi-base/Linux_Base.mof
                                -m ${MOF_FILE}
                                ${GENERATE_PROVIDERS}
                                ${CIM_CLASSES}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        RESULT_VARIABLE RES
                        OUTPUT_VARIABLE OUT
                        ERROR_VARIABLE ERR
                    )

        # Show error message when konkret fails
        if (NOT ${RES} EQUAL 0)
            message(FATAL_ERROR "KonkretCMPI failed: ${RES} ${ERR}")
        endif (NOT ${RES} EQUAL 0)

        # Move pregenerated sources for providers to source directory
        foreach(PROVIDER ${NEW_PROVIDERS})
            file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/${PROVIDER} ${CMAKE_CURRENT_SOURCE_DIR}/${PROVIDER})
        endforeach(PROVIDER ${NEW_PROVIDERS})

        # Return to caller
        set(${CIM_HEADERS} ${HEADERS})
        set(${CIM_PROVIDERS} ${PROVIDERS})
    endif (${LEN} EQUAL 0)
endmacro(konkretcmpi_generate MOF PROVIDERS HEADERS)

# This macro creates registration file from shared library
#
# @param[in] PROVIDER_NAME human-readable name of the provider
# @param[in] LIBRARY_NAME name of the library without lib prefix and .so suffix (same as for add_library)
# @param[in] MOF name of the MOF file
# @param[in] DEST destination directory where to install .reg file (use "" to skip installation)
#
macro(cim_registration PROVIDER_NAME LIBRARY_NAME MOF DEST)
    string(REPLACE ".mof" ".reg" REG ${MOF})
    # Create registration out of shared library
    add_custom_command(TARGET ${LIBRARY_NAME}
                       POST_BUILD
                       COMMAND ${KONKRETCMPI_KONKRETREG} lib${LIBRARY_NAME}.so > ${REG}
                       COMMENT "Generating .reg file from library for ${PROVIDER_NAME}"
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    # Install it
    if (NOT ${DEST} STREQUAL "")
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${REG} DESTINATION ${DEST})
    endif (NOT ${DEST} STREQUAL "")

    # Add custom target for registration
    find_file(MOF_FILE
              ${MOF}
              PATHS ${CMAKE_SOURCE_DIR}/mof/
    )

    find_file(OPENLMI_MOF_REGISTER
              openlmi-mof-register
              PATHS ${CMAKE_SOURCE_DIR}
              /usr/bin
    )
    add_custom_target(register-${PROVIDER_NAME}
                      ${OPENLMI_MOF_REGISTER} register ${MOF_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${REG})
    add_custom_target(unregister-${PROVIDER_NAME}
                      ${OPENLMI_MOF_REGISTER} unregister ${MOF_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${REG})
endmacro(cim_registration)
