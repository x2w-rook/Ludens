#pragma once

#include <iostream>
#include "Core/RenderBase/Include/RDevice.h"
#include "Core/RenderBase/Include/RShader.h"
#include "Core/RenderBase/Include/RBuffer.h"
#include "Core/RenderBase/Include/RBinding.h"
#include "Core/RenderBase/Lib/RDeviceGL.h"
#include "Core/RenderBase/Lib/RBindingGL.h"

namespace LD {

	static inline bool MissingPipeline(RDeviceBase* device, RResult& result)
	{
		if (!device->BoundPipelineH)
		{
			result.Type = RResultType::ResourceMissing;
			result.ResourceMissing.MissingType = RResourceType::Pipeline;
			return true;
		}

		return false;
	}

	static inline bool BufferTypeMismatch(RBufferType expect, RBufferType actual, RResult& result)
	{
		if (expect != actual)
		{
			result.Type = RResultType::BufferTypeMismatch;
			result.BufferTypeMismatch.Expect = expect;
			result.BufferTypeMismatch.Actual = actual;
			return true;
		}

		return false;
	}

	static inline bool ShaderTypeMismatch(RShaderType expect, RShaderType actual, RResult& result)
	{
		if (expect != actual)
		{
			result.Type = RResultType::ShaderTypeMismatch;
			result.ShaderTypeMismatch.Expect = expect;
			result.ShaderTypeMismatch.Actual = actual;
			return true;
		}

		return false;
	}

	RResult CreateRenderDevice(RDevice& device, const RDeviceInfo& info)
	{
		RResult result;

		if (device)
		{
			result.Type = RResultType::InvalidHandle;
			return result;
		}

		switch (info.Backend)
		{
		case RBackend::OpenGL:
			result = RDeviceGL::CreateRenderDevice(device, info);
			break;
		default:
			LD_DEBUG_UNREACHABLE;
		}

		return result;
	}

	RResult DeleteRenderDevice(RDevice& device)
	{
		RResult result;

		if (!device)
		{
			result.Type = RResultType::InvalidHandle;
			return result;
		}

		switch (device.GetBackend())
		{
		case RBackend::OpenGL:
			result = RDeviceGL::DeleteRenderDevice(device);
			break;
		default:
			LD_DEBUG_UNREACHABLE;
		}

		return result;
	}

	RResult RDevice::CreateTexture(RTexture& texture, const RTextureInfo& info)
	{
		RResult result;

		if (texture)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->CreateTexture(texture, info);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DeleteTexture(RTexture& texture)
	{
		RResult result;

		if (!texture)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->DeleteTexture(texture);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::CreateBuffer(RBuffer& buffer, const RBufferInfo& info)
	{
		RResult result;

		if (buffer)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->CreateBuffer(buffer, info);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DeleteBuffer(RBuffer& buffer)
	{
		RResult result;

		if (!buffer)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->DeleteBuffer(buffer);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::CreateShader(RShader& shader, const RShaderInfo& info)
	{
		RResult result;

		if (shader)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->CreateShader(shader, info);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DeleteShader(RShader& shader)
	{
		RResult result;
		
		if (!shader)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->DeleteShader(shader);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::CreateFrameBuffer(RFrameBuffer& frameBuffer, const RFrameBufferInfo& info)
	{
		RResult result;

		if (frameBuffer)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->CreateFrameBuffer(frameBuffer, info);
		
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DeleteFrameBuffer(RFrameBuffer& frameBuffer)
	{
		RResult result;

		if (!frameBuffer)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->DeleteFrameBuffer(frameBuffer);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::CreateBindingGroupLayout(RBindingGroupLayout& layout, const RBindingGroupLayoutInfo& info)
	{
		RResult result;
		
		if (layout)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->CreateBindingGroupLayout(layout, info);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DeleteBindingGroupLayout(RBindingGroupLayout& layout)
	{
		RResult result;

		if (!layout)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->DeleteBindingGroupLayout(layout);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::CreateBindingGroup(RBindingGroup& group, const RBindingGroupInfo& info)
	{
		RResult result;

		if (group)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->CreateBindingGroup(group, info);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DeleteBindingGroup(RBindingGroup& group)
	{
		RResult result;

		if (!group)
			result.Type = RResultType::InvalidHandle;
		else
			result = mDevice->DeleteBindingGroup(group);

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::CreatePipeline(RPipeline& pipeline, const RPipelineInfo& info)
	{
		RResult result;

		if (pipeline)
		{
			result.Type = RResultType::InvalidHandle;
			mDevice->Callback(result);
			return result;
		}

		if (!info.VertexShader || !info.FragmentShader)
		{
			result.Type = RResultType::ResourceMissing;
			result.ResourceMissing.MissingType = RResourceType::Shader;
			mDevice->Callback(result);
			return result;
		}

		RShaderBase& vs = Unwrap(info.VertexShader);
		RShaderBase& fs = Unwrap(info.FragmentShader);

		if (ShaderTypeMismatch(RShaderType::VertexShader, vs.Type, result) ||
			ShaderTypeMismatch(RShaderType::FragmentShader, fs.Type, result))
		{
			mDevice->Callback(result);
			return result;
		}

		result = mDevice->CreatePipeline(pipeline, info);
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DeletePipeline(RPipeline& pipeline)
	{
		RResult result;
		
		if (!pipeline)
		{
			result.Type = RResultType::InvalidHandle;
			mDevice->Callback(result);
			return result;
		}

		// if we are deleting the bound pipeline, unbind it first
		if (pipeline == mDevice->BoundPipelineH)
		{
			mDevice->BoundPipelineH.Reset();
		}
		
		result = mDevice->DeletePipeline(pipeline);
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::SetPipeline(RPipeline& pipelineH)
	{
		RResult result;

		if (mDevice->BoundPipelineH == pipelineH)
		{
			mDevice->Callback(result);
			return result;
		}

		mDevice->BoundPipelineH = pipelineH;
		result = mDevice->SetPipeline(pipelineH);
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::SetBindingGroup(u32 slot, RBindingGroup& groupH)
	{
		RResult result;

		// compare binding group layout with pipeline layout
		if (mDevice->BoundPipelineH)
		{
			RPipelineBase& pipeline = Unwrap(mDevice->BoundPipelineH);
			RBindingGroupBase& group = Unwrap(groupH);
			RBindingGroupLayoutBase& layout = Unwrap(group.GroupLayoutH);
			size_t groupCount = pipeline.GroupLayoutsH.Size();

			if (slot >= groupCount)
			{
				result.Type = RResultType::InvalidIndex;
				mDevice->Callback(result);
				return result;
			}

			RBindingGroupLayoutBase& slotLayout = Unwrap(pipeline.GroupLayoutsH[slot]);

			if (!layout.HasSameLayout(slotLayout))
			{
				result.Type = RResultType::BindingGroupMismatch;
				mDevice->Callback(result);
				return result;
			}
		}

		result = mDevice->SetBindingGroup(slot, groupH);
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::SetVertexBuffer(u32 slot, RBuffer& bufferH)
	{
		RResult result;

		if (MissingPipeline(mDevice, result))
		{
			mDevice->Callback(result);
			return result;
		}

		// TODO: bufferH invalid handle check?
		RBufferBase& buffer = Unwrap(bufferH);

		if (BufferTypeMismatch(RBufferType::VertexBuffer, buffer.Type, result))
		{
			mDevice->Callback(result);
			return result;
		}

		RPipelineBase& pipeline = Unwrap(mDevice->BoundPipelineH);
		size_t vertexBufferSlotCount = pipeline.VertexLayout.Slots.Size();
		
		if (slot >= vertexBufferSlotCount)
		{
			result.Type = RResultType::InvalidIndex;
			mDevice->Callback(result);
			return result;
		}

		result = mDevice->SetVertexBuffer(slot, bufferH);
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::SetIndexBuffer(RBuffer& bufferH)
	{
		RResult result;

		if (MissingPipeline(mDevice, result))
		{
			mDevice->Callback(result);
			return result;
		}

		// TODO: bufferH invalid handle
		RBufferBase& buffer = Unwrap(bufferH);
		
		if (BufferTypeMismatch(RBufferType::IndexBuffer, buffer.Type, result))
		{
			mDevice->Callback(result);
			return result;
		}

		result = mDevice->SetIndexBuffer(bufferH);
		mDevice->Callback(result);
		return result;
	}
	
	// NOTE: temporary solution, will remove later.
	RResult RDevice::SetFrameBuffer(RFrameBuffer* frameBuffer)
	{
		RResult result = mDevice->SetFrameBuffer(frameBuffer);
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::BeginDrawStats(RDrawStats* stats)
	{
		RResult result;

		LD_DEBUG_ASSERT(stats != nullptr);
		stats->TotalVertices = 0;
		stats->DrawVertexCalls = 0;
		stats->DrawIndexedCalls = 0;

		mDevice->Stats = stats;
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::EndDrawStats()
	{
		RResult result;

		mDevice->Stats = nullptr;
		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DrawVertex(const RDrawVertexInfo& info)
	{
		RResult result;

		if (MissingPipeline(mDevice, result))
		{
			mDevice->Callback(result);
			return result;
		}

		result = mDevice->DrawVertex(info);
		if (result && mDevice->Stats)
		{
			mDevice->Stats->DrawVertexCalls++;
			mDevice->Stats->TotalVertices += info.VertexCount;
		}

		mDevice->Callback(result);
		return result;
	}

	RResult RDevice::DrawIndexed(const RDrawIndexedInfo& info)
	{
		RResult result;

		if (MissingPipeline(mDevice, result))
		{
			mDevice->Callback(result);
			return result;
		}

		result = mDevice->DrawIndexed(info);
		if (result && mDevice->Stats)
		{
			mDevice->Stats->DrawIndexedCalls++;
			mDevice->Stats->TotalVertices += info.IndexCount;
		}

		mDevice->Callback(result);
		return result;
	}

} // namespace LD