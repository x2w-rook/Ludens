set(MODULE_INCLUDE
	"Include/Passes/GBufferPass.h"
	"Include/Passes/SSAOPass.h"
	"Include/FrameBuffers/GBuffer.h"
	"Include/FrameBuffers/SSAOBuffer.h"
	"Include/Groups/FrameStaticGroup.h"
	"Include/Groups/MaterialGroup.h"
	"Include/Groups/ViewportGroup.h"
	"Include/Groups/RectGroup.h"
	"Include/Groups/SSAOGroup.h"
	"Include/Pipelines/RectPipeline.h"
	"Include/Pipelines/GBufferPipeline.h"
	"Include/Pipelines/DeferredBlinnPhongPipeline.h"
	"Include/Pipelines/DeferredSSAOPipeline.h"
	"Include/PrefabRenderPass.h"
	"Include/PrefabBindingGroup.h"
	"Include/PrefabFrameBuffer.h"
	"Include/PrefabPipeline.h"
	"Include/RShaderCompiler.h"
	"Include/RMesh.h"
	"Include/RFont.h"
	"Include/RBatch.h"
)

set(MODULE_LIB
	"Lib/Passes/GBufferPass.cpp"
	"Lib/Passes/SSAOPass.cpp"
	"Lib/FrameBuffers/GBuffer.cpp"
	"Lib/FrameBuffers/SSAOBuffer.cpp"
	"Lib/Groups/FrameStaticGroup.cpp"
	"Lib/Groups/MaterialGroup.cpp"
	"Lib/Groups/ViewportGroup.cpp"
	"Lib/Groups/RectGroup.cpp"
	"Lib/Groups/SSAOGroup.cpp"
	"Lib/Pipelines/RectPipeline.cpp"
	"Lib/Pipelines/GBufferPipeline.cpp"
	"Lib/Pipelines/DeferredBlinnPhongPipeline.cpp"
	"Lib/Pipelines/DeferredSSAOPipeline.cpp"
	"Lib/RShaderCompiler.cpp"
	"Lib/RMesh.cpp"
	"Lib/RFont.cpp"
)

set(MODULE_INCLUDE_DIR
	"${CMAKE_SOURCE_DIR}/Ludens"
)

add_library(LDRenderFX STATIC
	"${MODULE_INCLUDE}"
	"${MODULE_LIB}"
)

target_link_libraries(LDRenderFX PRIVATE
	stb
)

target_include_directories(LDRenderFX PRIVATE
	"${MODULE_INCLUDE_DIR}"
)
