#include "Core/Header/Include/Error.h"
#include "Core/RenderBase/Include/GL/GLTexture.h"
#include "Core/RenderBase/Include/GL/GLContext.h"

namespace LD {

	GLTexture2D::GLTexture2D()
		: mContext(nullptr)
	{
	}

	GLTexture2D::~GLTexture2D()
	{
		LD_DEBUG_ASSERT(mHandle == 0);
	}

	void GLTexture2D::Setup(GLContext& context, const GLTexture2DInfo& info)
	{
		mHandle = CUID<GLTexture2D>::Get();
		mContext = &context;

		glCreateTextures(GL_TEXTURE_2D, 1, &mTexture);
		Bind(0);

		// TODO: these are technically sampler parameters, not texture parameters.
		//       For now we are using hard coded default values.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		LD_DEBUG_ASSERT(info.Data != nullptr);
		
		glTexImage2D(GL_TEXTURE_2D, 0, info.InternalFormat, (GLsizei)info.Width, (GLsizei)info.Height, 0, info.DataFormat, GL_UNSIGNED_BYTE, info.Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void GLTexture2D::Cleanup()
	{
		glDeleteTextures(1, &mTexture);

		mHandle.Reset();
		mContext = nullptr;
	}

	void GLTexture2D::Bind(int unit)
	{
		LD_DEBUG_ASSERT(mContext != nullptr);

		mContext->BindTextureUnit(unit);
		mContext->BindTexture2D(*this);
	}

} // namespace LD