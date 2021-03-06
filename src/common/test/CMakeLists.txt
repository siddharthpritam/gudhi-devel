project(Common_tests)

include(GUDHI_test_coverage)

add_executable ( Common_test_points_off_reader test_points_off_reader.cpp )
target_link_libraries(Common_test_points_off_reader ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable ( Common_test_distance_matrix_reader test_distance_matrix_reader.cpp )
target_link_libraries(Common_test_distance_matrix_reader ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable ( Common_test_persistence_intervals_reader test_persistence_intervals_reader.cpp )
target_link_libraries(Common_test_persistence_intervals_reader ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# Do not forget to copy test files in current binary dir
file(COPY "${CMAKE_SOURCE_DIR}/data/points/alphacomplexdoc.off" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY "${CMAKE_SOURCE_DIR}/data/distance_matrix/lower_triangular_distance_matrix.csv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY "${CMAKE_SOURCE_DIR}/data/distance_matrix/full_square_distance_matrix.csv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY "${CMAKE_SOURCE_DIR}/src/common/test/persistence_intervals_with_dimension.pers" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY "${CMAKE_SOURCE_DIR}/src/common/test/persistence_intervals_with_field.pers" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY "${CMAKE_SOURCE_DIR}/src/common/test/persistence_intervals_without_dimension.pers" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

gudhi_add_coverage_test(Common_test_points_off_reader)
gudhi_add_coverage_test(Common_test_distance_matrix_reader)
gudhi_add_coverage_test(Common_test_persistence_intervals_reader)
