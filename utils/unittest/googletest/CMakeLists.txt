SET(GTEST_DIR ${PROJECT_SOURCE_DIR}/utils/unittest/googletest)
SET(GTEST_SOURCES
  ${GTEST_DIR}/gtest-death-test.cc
  ${GTEST_DIR}/gtest-filepath.cc
  ${GTEST_DIR}/gtest-port.cc
  ${GTEST_DIR}/gtest-printers.cc
  ${GTEST_DIR}/gtest-test-part.cc
  ${GTEST_DIR}/gtest-typed-test.cc
  ${GTEST_DIR}/gtest.cc
)
SOURCE_GROUP(utils\\unittest\\googletest FILES ${GTEST_SOURCES})

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(gtest STATIC ${GTEST_SOURCES})

IF(MSVC)
  ADD_DEFINITIONS(-D_VARIADIC_MAX=10)
ENDIF(MSVC)
