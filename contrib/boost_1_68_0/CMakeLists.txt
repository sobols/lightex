#
# boost (header-only)
#
add_library(boost INTERFACE)
target_include_directories(boost INTERFACE .)
target_compile_definitions(boost INTERFACE -DBOOST_SPIRIT_UNICODE -DBOOST_SPIRIT_X3_UNICODE)

#
# boost_test
#
add_library(boost_test STATIC
    libs/test/src/compiler_log_formatter.cpp
    libs/test/src/debug.cpp
    libs/test/src/decorator.cpp
    libs/test/src/execution_monitor.cpp
    libs/test/src/framework.cpp
    libs/test/src/junit_log_formatter.cpp
    libs/test/src/plain_report_formatter.cpp
    libs/test/src/progress_monitor.cpp
    libs/test/src/results_collector.cpp
    libs/test/src/results_reporter.cpp
    libs/test/src/test_framework_init_observer.cpp
    libs/test/src/test_tools.cpp
    libs/test/src/test_tree.cpp
    libs/test/src/unit_test_log.cpp
    libs/test/src/unit_test_main.cpp
    libs/test/src/unit_test_monitor.cpp
    libs/test/src/unit_test_parameters.cpp
    libs/test/src/xml_log_formatter.cpp
    libs/test/src/xml_report_formatter.cpp
)
target_link_libraries(boost_test boost)
