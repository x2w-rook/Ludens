#include "Core/RenderBase/Lib/RPipelineGL.h"
#include "Core/RenderBase/Lib/RShaderGL.h"
#include "Core/RenderBase/Lib/RDeriveGL.h"
#include "Core/RenderBase/Lib/RDeviceGL.h"

namespace LD {

	RPipelineGL::RPipelineGL()
	{
	}

	RPipelineGL::~RPipelineGL()
	{
		LD_DEBUG_ASSERT(ID == 0);
	}

	void RPipelineGL::Setup(RPipeline& pipelineH, const RPipelineInfo& info, RDeviceGL& device)
	{
		RPipelineBase::Setup(pipelineH, info, &device);

		PrimitiveTopology = DeriveGLPrimitiveTopology(info.PrimitiveTopology);

		RShaderGL& vertexShader = Derive<RShaderGL>(VertexShaderH);
		RShaderGL& fragmentShader = Derive<RShaderGL>(FragmentShaderH);

		// TODO: SPIR-V compile path

		GLProgramInfo programInfo{};
		programInfo.VertexShaderData = vertexShader.Source.data();
		programInfo.VertexShaderSize = vertexShader.Source.size();
		programInfo.FragmentShaderData = fragmentShader.Source.data();
		programInfo.FragmentShaderSize = fragmentShader.Source.size();
		Program.Setup(device.Context, programInfo);

		// One VAO per pipeline is used to capture how vertex attributes are polled from one or more vertex buffers.
		VAO.Setup(device.Context);
		VAO.Bind();

		u32 vertexBufferSlotCount = info.VertexLayout.Slots.Size();
		VertexStrides.Resize(vertexBufferSlotCount);

		for (size_t slotIdx = 0; slotIdx < vertexBufferSlotCount; slotIdx++)
		{
			const RVertexBufferSlot& slot = info.VertexLayout.Slots[slotIdx];

			GLVertexLayout layout;
			DeriveGLVertexLayout(slot, layout);
			VertexStrides[slotIdx] = layout.GetVertexStride();
			const Vector<GLVertexAttribute>& attrs = layout.GetAttributes();
			const Vector<u32>& offsets = layout.GetOffsets();
			
			for (size_t attrIdx = 0; attrIdx < attrs.Size(); attrIdx++)
			{
				const GLVertexAttribute& attr = attrs[attrIdx];
				GLint attrComponentCount;
				GLenum attrComponentType;
				u32 attrByteSize;
				GetGLSLTypeVertexAttribute(attr.Type, &attrComponentCount, &attrComponentType, &attrByteSize);

				glEnableVertexAttribArray(attr.Location);
				glVertexAttribFormat(attr.Location, attrComponentCount, attrComponentType, attr.Normalize, offsets[attrIdx]);
				glVertexAttribBinding(attr.Location, slotIdx);
			}
		}
	}

	void RPipelineGL::Cleanup(RPipeline& pipelineH)
	{
		RPipelineBase::Cleanup(pipelineH);

		VAO.Cleanup();
		Program.Cleanup();
	}

} // namespace LD