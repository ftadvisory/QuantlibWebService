cmake_minimum_required(VERSION 3.1)

set(project_name QuantlibWebService) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/SwapController.cpp
        src/controller/SwapController.hpp
        src/dto/DTOs.hpp
)

## link libs

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED) # <-- add this

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger         # <-- add this
        PUBLIC oatpp::oatpp-test
)

link_directories( /usr/local/lib/ )
link_libraries(Quantlib)
## define path to swagger-ui res folder

add_definitions( 
-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"
) # <-- add this

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}
        src/App.cpp
        test/app/MyApiTestClient.hpp)
target_link_libraries(${project_name} ${project_name}-lib)
add_dependencies(${project_name} ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/app/TestComponent.hpp
        test/app/MyApiTestClient.hpp
        test/SwapControllerTest.cpp
        test/SwapControllerTest.hpp
)

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name} ${project_name}-test PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)
