# This function adds resources for your target, the behaviour depends on the current CMAKE_SYSTEM_NAME.
function(target_add_resources target_name resource_in resource_out)
    if (CMAKE_SYSTEM_NAME STREQUAL Android)
        message(WARNING "target_add_resources: Android not yet implemented")
    elseif (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
        message(STATUS "target_add_resources: embedding resources "
                "target=${target_name} resources_in=${resource_in} resource_out=${resource_out}")
        target_link_options(${target_name} PRIVATE --embed-file ${resource_in}@${resource_out})
    else ()
        message(STATUS "target_add_resources: copying resources"
                "target=${target_name} resources_in=${resource_in} resource_out=${resource_out}")
        file(COPY ${resource_in} DESTINATION ${resource_out})
    endif ()
endfunction()
