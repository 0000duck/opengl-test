cmake_minimum_required(VERSION 3.0)
project(app1)

option(AMD64 ON)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(app1/lib/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMB_BUILD_AC_IMPORTER OFF)
option(ASSIMB_BUILD_AMF_IMPORTER OFF)
option(ASSIMB_BUILD_ASE_IMPORTER OFF)
option(ASSIMB_BUILD_ASSBIN_IMPORTER OFF)
option(ASSIMB_BUILD_ASSXML_IMPORTER OFF)
option(ASSIMB_BUILD_B3D_IMPORTER OFF)
option(ASSIMB_BUILD_BLEND_IMPORTER OFF)
option(ASSIMB_BUILD_BVH_IMPORTER OFF)
option(ASSIMB_BUILD_COB_IMPORTER OFF)
option(ASSIMB_BUILD_CSM_IMPORTER OFF)
option(ASSIMB_BUILD_DXF_IMPORTER OFF)
option(ASSIMB_BUILD_FBX_IMPORTER OFF)
option(ASSIMB_BUILD_GLTF_IMPORTER OFF)
option(ASSIMB_BUILD_HMP_IMPORTER OFF)
option(ASSIMB_BUILD_IFC_IMPORTER OFF)
option(ASSIMB_BUILD_IRRMESH_IMPORTER OFF)
option(ASSIMB_BUILD_IRR_IMPORTER OFF)
option(ASSIMB_BUILD_LWO_IMPORTER OFF)
option(ASSIMB_BUILD_LWS_IMPORTER OFF)
option(ASSIMB_BUILD_MD2_IMPORTER OFF)
option(ASSIMB_BUILD_MD3_IMPORTER OFF)
option(ASSIMB_BUILD_MD5_IMPORTER OFF)
option(ASSIMB_BUILD_MS3D_IMPORTER OFF)
option(ASSIMB_BUILD_NDO_IMPORTER OFF)
option(ASSIMB_BUILD_NFF_IMPORTER OFF)
option(ASSIMB_BUILD_NONFREE_C4D_IMPORTER OFF)
option(ASSIMB_BUILD_OFF_IMPORTER OFF)
option(ASSIMB_BUILD_OGRE_IMPORTER OFF)
option(ASSIMB_BUILD_OPENGEX_IMPORTER OFF)
option(ASSIMB_BUILD_PLY_IMPORTER OFF)
option(ASSIMB_BUILD_Q3BSP_IMPORTER OFF)
option(ASSIMB_BUILD_Q3D_IMPORTER OFF)
option(ASSIMB_BUILD_SIB_IMPORTER OFF)
option(ASSIMB_BUILD_SMD_IMPORTER OFF)
option(ASSIMB_BUILD_TERRAGEN_IMPORTER OFF)
option(ASSIMB_BUILD_X3D_IMPORTER OFF)
option(ASSIMB_BUILD_XGL_IMPORTER OFF)
option(ASSIMB_BUILD_X_IMPORTER OFF)

add_subdirectory(app1/lib/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
#add_subdirectory(app1/lib/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++14")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(app1/src/
                    app1/lib/assimp/include/
                    #app1/lib/bullet/src/
                    app1/lib/glad/include/
                    app1/lib/glfw/include/
                    app1/lib/glm/
                    app1/lib/PerlinNoise
                    #app1/lib/stb/
        )

file(GLOB VENDORS_SOURCES app1/lib/glad/src/glad.c)
file(GLOB PROJECT_HEADERS app1/src/*.hpp)
file(GLOB PROJECT_SOURCES app1/src/*.cpp)
file(GLOB PROJECT_SHADERS app1/shaders/*.comp
                          app1/shaders/*.frag
                          app1/shaders/*.geom
                          app1/shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
                -DSHADERS_DIR=\"${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/shaders/\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME}
        assimp
        glfw
        ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
        #BulletDynamics BulletCollision
        )#LinearMath)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
