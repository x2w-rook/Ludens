#include <unordered_set>
#include "Core/DSA/Include/Array.h"
#include "Core/DSA/Include/Vector.h"
#include "Core/RenderBase/Include/GL/GL.h"
#include "Core/RenderBase/Lib/RDeriveGL.h"
#include "Core/RenderBase/Lib/RDeviceGL.h"
#include "Core/RenderBase/Lib/RTextureGL.h"
#include "Core/RenderBase/Lib/RBufferGL.h"
#include "Core/RenderBase/Lib/RShaderGL.h"
#include "Core/RenderBase/Lib/RFrameBufferGL.h"
#include "Core/RenderBase/Lib/RBindingGL.h"
#include "Core/RenderBase/Lib/RPipelineGL.h"

#define MAX_BUFFER_COUNT     1024
#define MAX_TEXTURE_COUNT    1024
#define MAX_SHADER_COUNT     1024
#define MAX_FRAME_BUFFER_COUNT           256
#define MAX_BINDING_GROUP_LAYOUT_COUNT   512
#define MAX_BINDING_GROUP_COUNT          512
#define MAX_PIPELINE_COUNT               512

namespace LD {

	static RDeviceGL sDevice;

	RDeviceGL::RDeviceGL()
	{
	}

	RDeviceGL::~RDeviceGL()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RDeviceGL::Setup(RDevice& handle, const RDeviceInfo& info)
	{
		RDeviceBase::Setup(handle, info);
		handle.mBackend = RBackend::OpenGL;

		Context.Setup();

		TextureAllocator.Setup(MAX_TEXTURE_COUNT);
		BufferAllocator.Setup(MAX_BUFFER_COUNT);
		ShaderAllocator.Setup(MAX_SHADER_COUNT);
		FrameBufferAllocator.Setup(MAX_FRAME_BUFFER_COUNT);
		BindingGroupLayoutAllocator.Setup(MAX_BINDING_GROUP_LAYOUT_COUNT);
		BindingGroupAllocator.Setup(MAX_BINDING_GROUP_COUNT);
		PipelineAllocator.Setup(MAX_PIPELINE_COUNT);
	}

	void RDeviceGL::Cleanup(RDevice& handle)
	{
		RDeviceBase::Cleanup(handle);
		
		Context.Cleanup();

		PipelineAllocator.Cleanup();
		BindingGroupAllocator.Cleanup();
		BindingGroupLayoutAllocator.Cleanup();
		FrameBufferAllocator.Cleanup();
		ShaderAllocator.Cleanup();
		BufferAllocator.Cleanup();
		TextureAllocator.Cleanup();
	}

	RResult RDeviceGL::CreateRenderDevice(RDevice& deviceH, const RDeviceInfo& info)
	{
		LD_DEBUG_ASSERT((UID)sDevice.ID == 0 && "multi device is not yet implemented");
		LD_DEBUG_ASSERT(info.Backend == RBackend::OpenGL);
		
		sDevice.Setup(deviceH, info);

		return {};
	}

	RResult RDeviceGL::DeleteRenderDevice(RDevice& deviceH)
	{
		LD_DEBUG_ASSERT((UID)sDevice.ID == deviceH.GetID());

		sDevice.Cleanup(deviceH);

		return {};
	}

	RResult RDeviceGL::CreateTexture(RTexture& textureH, const RTextureInfo& info)
	{
		RTextureGL* texture = (RTextureGL*)TextureAllocator.Alloc(sizeof(RTextureGL));
		new (texture) RTextureGL{};
		texture->Setup(textureH, info, *this);

		return {};
	}

	RResult RDeviceGL::DeleteTexture(RTexture& textureH)
	{
		RTextureGL& texture = Derive<RTextureGL>(textureH);

		texture.Cleanup(textureH);
		texture.~RTextureGL();
		TextureAllocator.Free(&texture);

		return {};
	}

	RResult RDeviceGL::CreateBuffer(RBuffer& bufferH, const RBufferInfo& info)
	{
		RBufferGL* buffer = (RBufferGL*)BufferAllocator.Alloc(sizeof(RBufferGL));
		new (buffer) RBufferGL{};
		buffer->Setup(bufferH, info, *this);

		return {};
	}

	RResult RDeviceGL::DeleteBuffer(RBuffer& bufferH)
	{
		RBufferGL* buffer = dynamic_cast<RBufferGL*>((RBufferBase*)bufferH);

		buffer->Cleanup(bufferH);
		buffer->~RBufferGL();
		BufferAllocator.Free(buffer);

		return {};
	}

	RResult RDeviceGL::CreateShader(RShader& shaderH, const RShaderInfo& info)
	{
		RShaderGL* shader = (RShaderGL*)ShaderAllocator.Alloc(sizeof(RShaderGL));
		new (shader) RShaderGL{};
		shader->Setup(shaderH, info, *this);

		return {};
	}

	RResult RDeviceGL::DeleteShader(RShader& shaderH)
	{
		RShaderGL* shader = dynamic_cast<RShaderGL*>((RShaderBase*)shaderH);

		shader->Cleanup(shaderH);
		shader->~RShaderGL();
		ShaderAllocator.Free(shader);

		return {};
	}

	RResult RDeviceGL::CreateFrameBuffer(RFrameBuffer& frameBufferH, const RFrameBufferInfo& info)
	{
		RFrameBufferGL* frameBuffer = (RFrameBufferGL*)FrameBufferAllocator.Alloc(sizeof(RFrameBufferGL));
		new (frameBuffer) RFrameBufferGL{};
		frameBuffer->Setup(frameBufferH, info, *this);

		return {};
	}

	RResult RDeviceGL::DeleteFrameBuffer(RFrameBuffer& frameBufferH)
	{
		RFrameBufferGL& frameBuffer = Derive<RFrameBufferGL>(frameBufferH);

		frameBuffer.Cleanup(frameBufferH);
		frameBuffer.~RFrameBufferGL();
		FrameBufferAllocator.Free(&frameBuffer);
		
		return {};
	}

	RResult RDeviceGL::CreateBindingGroupLayout(RBindingGroupLayout& layoutH, const RBindingGroupLayoutInfo& info)
	{
		RBindingGroupLayoutGL* layout = (RBindingGroupLayoutGL*)BindingGroupLayoutAllocator.Alloc(sizeof(RBindingGroupLayoutGL));
		new (layout) RBindingGroupLayoutGL{};
		layout->Setup(layoutH, info, *this);

		return {};
	}

	RResult RDeviceGL::DeleteBindingGroupLayout(RBindingGroupLayout& layoutH)
	{
		RBindingGroupLayoutGL* layout = dynamic_cast<RBindingGroupLayoutGL*>((RBindingGroupLayoutBase*)layoutH);

		layout->Cleanup(layoutH);
		layout->~RBindingGroupLayoutGL();
		BindingGroupLayoutAllocator.Free(layout);

		return {};
	}

	RResult RDeviceGL::CreateBindingGroup(RBindingGroup& groupH, const RBindingGroupInfo& info)
	{
		RBindingGroupGL* group = (RBindingGroupGL*)BindingGroupAllocator.Alloc(sizeof(RBindingGroupGL));
		new (group) RBindingGroupGL{};
		group->Setup(groupH, info, *this);

		return {};
	}

	RResult RDeviceGL::DeleteBindingGroup(RBindingGroup& groupH)
	{
		RBindingGroupGL* group = dynamic_cast<RBindingGroupGL*>((RBindingGroupBase*)groupH);

		group->Cleanup(groupH);
		group->~RBindingGroupGL();
		BindingGroupAllocator.Free(group);

		return {};
	}

	RResult RDeviceGL::CreatePipeline(RPipeline& pipelineH, const RPipelineInfo& info)
	{
		RPipelineGL* pipeline = (RPipelineGL*)PipelineAllocator.Alloc(sizeof(RPipelineGL));
		new (pipeline) RPipelineGL{};
		pipeline->Setup(pipelineH, info, *this);

		return {};
	}

	RResult RDeviceGL::DeletePipeline(RPipeline& pipelineH)
	{
		RPipelineGL* pipeline = dynamic_cast<RPipelineGL*>((RPipelineBase*)pipelineH);

		pipeline->Cleanup(pipelineH);
		pipeline->~RPipelineGL();
		PipelineAllocator.Free(pipeline);

		return {};
	}

	RResult RDeviceGL::SetPipeline(RPipeline& pipelineH)
	{
		LD_DEBUG_ASSERT(BoundPipelineH && BoundPipelineH == pipelineH);

		RPipelineGL& pipeline = Derive<RPipelineGL>(BoundPipelineH);
		pipeline.VAO.Bind();
		pipeline.Program.Bind();

		// TODO: reset binding group

		return {};
	}

	RResult RDeviceGL::SetBindingGroup(u32 groupIdx, RBindingGroup& groupH)
	{
		LD_DEBUG_ASSERT(BoundPipelineH);

		RBindingGroupGL& group = Derive<RBindingGroupGL>(groupH);
		RPipelineGL& pipeline = Derive<RPipelineGL>(BoundPipelineH);

		// look up table that maps binding qualifiers to OpenGL texture units or buffer bases
		std::unordered_map<u32, u32>& textureUnitBinding = pipeline.TextureUnitBinding[groupIdx];
		std::unordered_map<u32, u32>& uniformBufferBinding = pipeline.UniformBufferBinding[groupIdx];
		
		// TODO: compare with bound pipeline layout
		pipeline.Program.Bind();

		for (size_t bindingIdx = 0; bindingIdx < group.Bindings.Size(); bindingIdx++)
		{
			RBindingGroupBase::Binding& binding = group.Bindings[bindingIdx];

			switch (binding.Type)
			{
			case RBindingType::Texture:
			{
				if (!binding.TextureH)
					break;

				RTextureGL& texture = Derive<RTextureGL>(binding.TextureH);

				// bind OpenGL texture at correct unit
				int textureUnit = (int)textureUnitBinding[bindingIdx];
				texture.Bind(textureUnit);
				break;
			}
			case RBindingType::UniformBuffer:
			{
				if (!binding.BufferH)
					break;

				RBufferGL& buffer = Derive<RBufferGL>(binding.BufferH);
				LD_DEBUG_ASSERT(buffer.Target == GL_UNIFORM_BUFFER);
				
				// bind OpenGL UBO at correct uniform buffer base
				int bufferBase = (int)uniformBufferBinding[bindingIdx];
				buffer.UBO.BindBase(bufferBase);
				break;
			}
			default:
				LD_DEBUG_UNREACHABLE;
			}
		}

		return {};
	}

	RResult RDeviceGL::SetVertexBuffer(u32 slot, RBuffer& bufferH)
	{
		RPipelineGL& pipeline = Derive<RPipelineGL>(BoundPipelineH);
		RBufferGL& buffer = Derive<RBufferGL>(bufferH);

		LD_DEBUG_ASSERT(buffer.Target == GL_ARRAY_BUFFER && "binding a non vertex buffer");
		
		pipeline.VAO.Bind();

		glBindVertexBuffer(slot, (GLuint)buffer.VBO, 0, pipeline.VertexStrides[slot]);

		return {};
	}

	RResult RDeviceGL::SetIndexBuffer(RBuffer& bufferH)
	{
		RBufferGL& buffer = Derive<RBufferGL>(bufferH);

		LD_DEBUG_ASSERT(buffer.Target == GL_ELEMENT_ARRAY_BUFFER && "binding a non index buffer");

		buffer.Bind();
	
		return {};
	}

	RResult RDeviceGL::SetFrameBuffer(RFrameBuffer* frameBufferH)
	{
		if (frameBufferH == nullptr)
		{
			Context.UnbindFrameBuffer();
			return {};
		}

		RFrameBufferGL& frameBuffer = Derive<RFrameBufferGL>(*frameBufferH);
		Context.BindFrameBuffer(frameBuffer.FBO);

		return {};
	}

	RResult RDeviceGL::DrawVertex(const RDrawVertexInfo& info)
	{
		LD_DEBUG_ASSERT(info.InstanceCount == 1);
		LD_DEBUG_ASSERT(BoundPipelineH);

		RPipelineGL& pipeline = Derive<RPipelineGL>(BoundPipelineH);

		GLCommand::DrawArrays(pipeline.PrimitiveTopology, info.VertexCount, info.VertexStart);

		return {};
	}

	RResult RDeviceGL::DrawIndexed(const RDrawIndexedInfo& info)
	{
		LD_DEBUG_ASSERT(info.InstanceCount == 1);
		LD_DEBUG_ASSERT(BoundPipelineH);

		RPipelineGL& pipeline = Derive<RPipelineGL>(BoundPipelineH);

		GLCommand::DrawElements(pipeline.PrimitiveTopology, info.IndexCount, DeriveGLIndexType(info.IndexType));

		return {};
	}

} // namespace LD