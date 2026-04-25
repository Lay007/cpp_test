option(DSP_ENABLE_AVX2 "Enable AVX2 SIMD kernels where available" OFF)

add_library(my_project_dsp STATIC
    src/dsp.cpp
)

target_include_directories(my_project_dsp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(my_project_dsp PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(my_project_dsp PRIVATE /W4)
else()
    target_compile_options(my_project_dsp PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(DSP_ENABLE_AVX2)
    target_compile_definitions(my_project_dsp PRIVATE MY_PROJECT_DSP_ENABLE_AVX2=1)
    if(MSVC)
        target_compile_options(my_project_dsp PRIVATE /arch:AVX2)
    else()
        target_compile_options(my_project_dsp PRIVATE -mavx2 -mfma)
    endif()
endif()

add_executable(my_project_demo src/dsp_demo.cpp)
target_link_libraries(my_project_demo PRIVATE my_project_dsp)

add_executable(my_project_dsp_benchmark bench/dsp_benchmark.cpp)
target_link_libraries(my_project_dsp_benchmark PRIVATE my_project_dsp)

add_executable(iq_analysis_demo src/iq_analysis_demo.cpp)
target_link_libraries(iq_analysis_demo PRIVATE my_project_dsp)

set(DSP_BENCHMARK_REPORT "${CMAKE_BINARY_DIR}/reports/dsp_performance.md")
add_custom_target(benchmark_report
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/reports"
    COMMAND $<TARGET_FILE:my_project_dsp_benchmark> --iterations 20 --report "${DSP_BENCHMARK_REPORT}"
    DEPENDS my_project_dsp_benchmark
    COMMENT "Generating DSP performance report at ${DSP_BENCHMARK_REPORT}"
    VERBATIM
)

if(BUILD_TESTING)
    add_executable(my_project_dsp_tests
        tests/dsp_fir_test.cpp
        tests/dsp_goertzel_test.cpp
        tests/dsp_gcc_phat_test.cpp
        tests/dsp_resampler_test.cpp
    )
    target_link_libraries(my_project_dsp_tests PRIVATE my_project_dsp GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(my_project_dsp_tests
        DISCOVERY_TIMEOUT 60
        TEST_LIST my_project_dsp_discovered_tests
    )
    set_tests_properties(${my_project_dsp_discovered_tests} PROPERTIES LABELS "unit;dsp")

    add_test(NAME my_project_demo_smoke COMMAND my_project_demo)
    set_tests_properties(my_project_demo_smoke PROPERTIES LABELS "smoke;dsp")

    add_test(NAME my_project_dsp_perf
             COMMAND $<TARGET_FILE:my_project_dsp_benchmark> --iterations 5)
    set_tests_properties(my_project_dsp_perf PROPERTIES
        LABELS "perf;dsp"
        TIMEOUT 180
    )
endif()
