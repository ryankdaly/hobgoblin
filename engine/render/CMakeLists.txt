# ! many lines commented out here, included for demo of full codebase release
add_library(engine_render
    #src/renderer_d3d11.cpp
	src/mesh.cpp
	#src/texture.cpp
	#src/mesh_loader.cpp
	#src/text_renderer.cpp
	#src/crosshair_renderer.cpp
)

target_include_directories(engine_render
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
          ${CMAKE_CURRENT_SOURCE_DIR}/../extern
)


target_link_libraries(engine_render
    PUBLIC
        #engine_core
        #engine_platform
		glm::glm  
    PRIVATE
        d3d11
        dxgi
        D3DCompiler
)
