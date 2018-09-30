add_subdirectory(styles)

# Define lightex_root variable
get_filename_component(lightex_root .. ABSOLUTE)

# Define LighTeX static library
include_directories(${lightex_root})
set(lightex_src_files ${lightex_src_files}
    ${lightex_root}/lightex/workspace.cc
    ${lightex_root}/lightex/dot_converter/dot_visitor.cc
    ${lightex_root}/lightex/html_converter/html_visitor.cc
    ${lightex_root}/lightex/utils/file_utils.cc
    ${lightex_root}/lightex/utils/utf32.cc
)
add_library(lightex STATIC ${lightex_src_files})
target_link_libraries(lightex boost)

# Define LighTeX binaries
add_executable(parse_program_to_dot ${lightex_root}/lightex/binaries/parse_program_to_dot.cc)
target_link_libraries(parse_program_to_dot lightex)

add_executable(parse_program_to_html ${lightex_root}/lightex/binaries/parse_program_to_html.cc)
target_link_libraries(parse_program_to_html lightex styles)

# Unittest
add_executable(tester ${lightex_root}/lightex/tests/main.cc)
target_link_libraries(tester lightex boost_test)

# Python module

find_package(PythonLibs)
if(PYTHONLIBS_FOUND)
    message(STATUS "Python is found")
    include_directories("${PYTHON_INCLUDE_DIRS}")
    add_library(pylightex MODULE
        ${lightex_root}/lightex/pymod/extension.cc
        ${lightex_root}/lightex/pymod/extension.h
        ${lightex_root}/lightex/pymod/pylightex.cc
        ${lightex_root}/lightex/pymod/pylightex.h
    )
    target_link_libraries(pylightex lightex styles ${PYTHON_LIBRARIES})
    set_target_properties(pylightex PROPERTIES PREFIX "")
    if(WIN32)
        set_target_properties(pylightex PROPERTIES SUFFIX ".pyd")
    endif(WIN32)
else(PYTHONLIBS_FOUND)
    message(WARNING "Python is not found")
endif(PYTHONLIBS_FOUND) 
