add_library(dsp_core_dsp STATIC)
add_library(dsp_core::dsp ALIAS dsp_core_dsp)

target_sources(dsp_core_dsp
    PRIVATE
        src/dsp.cpp
)

target_include_directories(dsp_core_dsp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(dsp_core_dsp PUBLIC cxx_std_17)
set_target_properties(dsp_core_dsp PROPERTIES
    EXPORT_NAME dsp
    OUTPUT_NAME dsp_core_dsp
    POSITION_INDEPENDENT_CODE ON
)

function(dsp_core_enable_warnings target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 $<$<BOOL:${DSP_WARNINGS_AS_ERRORS}>:/WX>)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic $<$<BOOL:${DSP_WARNINGS_AS_ERRORS}>:-Werror>)
    endif()
endfunction()

dsp_core_enable_warnings(dsp_core_dsp)

if(DSP_ENABLE_AVX2)
    target_compile_definitions(dsp_core_dsp PRIVATE MY_PROJECT_DSP_ENABLE_AVX2=1)
    if(MSVC)
        target_compile_options(dsp_core_dsp PRIVATE /arch:AVX2)
    else()
        target_compile_options(dsp_core_dsp PRIVATE -mavx2 -mfma)
    endif()
endif()

if(DSP_BUILD_DEMOS)
    add_executable(dsp_core_demo src/dsp_demo.cpp)
    target_link_libraries(dsp_core_demo PRIVATE dsp_core::dsp)
    dsp_core_enable_warnings(dsp_core_demo)

    add_executable(iq_analysis_demo src/iq_analysis_demo.cpp)
    target_link_libraries(iq_analysis_demo PRIVATE dsp_core::dsp)
    dsp_core_enable_warnings(iq_analysis_demo)
endif()

if(DSP_BUILD_BENCHMARKS)
    add_executable(dsp_core_benchmark bench/dsp_benchmark.cpp)
    target_link_libraries(dsp_core_benchmark PRIVATE dsp_core::dsp)
    dsp_core_enable_warnings(dsp_core_benchmark)

    set(DSP_BENCHMARK_REPORT "${CMAKE_BINARY_DIR}/reports/dsp_performance.md")
    add_custom_target(benchmark_report
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/reports"
        COMMAND $<TARGET_FILE:dsp_core_benchmark> --iterations 20 --report "${DSP_BENCHMARK_REPORT}"
        DEPENDS dsp_core_benchmark
        COMMENT "Generating DSP performance report at ${DSP_BENCHMARK_REPORT}"
        VERBATIM
    )
endif()

if(BUILD_TESTING)
    add_executable(dsp_core_tests
        tests/dsp_fir_test.cpp
        tests/dsp_goertzel_test.cpp
        tests/dsp_gcc_phat_test.cpp
        tests/dsp_resampler_test.cpp
        tests/dsp_api_validation_test.cpp
    )
    target_link_libraries(dsp_core_tests PRIVATE dsp_core::dsp GTest::gtest_main)
    dsp_core_enable_warnings(dsp_core_tests)

    include(GoogleTest)
    gtest_discover_tests(dsp_core_tests
        DISCOVERY_TIMEOUT 60
        DISCOVERY_MODE PRE_TEST
        TEST_LIST dsp_core_discovered_tests
    )
    if(dsp_core_discovered_tests)
        set_tests_properties(${dsp_core_discovered_tests} PROPERTIES LABELS "unit;dsp")
    endif()

    if(DSP_BUILD_DEMOS)
        add_test(NAME dsp_core_demo_smoke COMMAND $<TARGET_FILE:dsp_core_demo>)
        set_tests_properties(dsp_core_demo_smoke PROPERTIES LABELS "smoke;dsp")
    endif()

    if(DSP_BUILD_BENCHMARKS)
        add_test(NAME dsp_core_perf
                 COMMAND $<TARGET_FILE:dsp_core_benchmark> --iterations 5)
        set_tests_properties(dsp_core_perf PROPERTIES
            LABELS "perf;dsp"
            TIMEOUT 180
        )
    endif()
endif()

install(TARGETS dsp_core_dsp
    EXPORT cpp_dsp_showcaseTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cpp_dsp_showcaseConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_dsp_showcaseConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_dsp_showcase
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_dsp_showcaseConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_dsp_showcaseConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_dsp_showcaseConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_dsp_showcase
)
install(EXPORT cpp_dsp_showcaseTargets
    FILE cpp_dsp_showcaseTargets.cmake
    NAMESPACE dsp_core::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_dsp_showcase
)
