# cmake/FilterByDepth.cmake

# Helper function to filter files based on directory depth
function(filter_by_depth input_files output_files min_depth)
    set(filtered_files "")
    foreach(file ${${input_files}})
        get_filename_component(file_dir ${file} PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" relative_path ${file_dir})
        string(REPLACE "/" ";" path_components ${relative_path})
        list(LENGTH path_components depth)
        if(depth GREATER ${min_depth})
            list(APPEND filtered_files ${file})
        endif()
    endforeach()
    set(${output_files} ${filtered_files} PARENT_SCOPE)
endfunction()
