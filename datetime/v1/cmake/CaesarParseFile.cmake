include_guard()

#[[
Parse the contents of the file specified by <filename> and store them in
<result>.

The input file is treated as a dependency of the CMake project. Modifications
to this file will trigger re-configuration in the next build.

If the input is a relative path, it is treated with respect to the value of
*CMAKE_CURRENT_SOURCE_DIR*.

``
caesar_parse_file(<filename> <result>)
``
#]]
function(caesar_parse_file filename result)
    # Read file contents to variable.
    file(STRINGS ${filename} contents)

    # Add *filename* as a dependency of the CMake build. CMake inspects the
    # timestamp of the file and will re-configure if the file has changed since
    # the last cmake run.
    configure_file(${filename} ${filename} COPYONLY)

    # Set output variable.
    set(${result} "${contents}" PARENT_SCOPE)
endfunction()
