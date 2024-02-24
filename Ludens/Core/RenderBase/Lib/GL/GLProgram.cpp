#include <iostream>
#include "Core/RenderBase/Include/GL/GLProgram.h"
#include "Core/RenderBase/Include/GL/GLContext.h"
#include "Core/Header/Include/Error.h"



namespace LD {

	GLProgram::GLProgram()
		: mContext(nullptr)
	{
	}

	GLProgram::~GLProgram()
	{
		LD_DEBUG_ASSERT(mHandle == 0);
	}

	void GLProgram::Setup(GLContext& context, const GLProgramInfo& info)
	{
		mHandle = CUID<GLProgram>::Get();
		mContext = &context;
		mVSData = info.VertexShaderData;
		mVSSize = info.VertexShaderSize;
		mFSData = info.FragmentShaderData;
		mFSSize = info.FragmentShaderSize;
		mProgram = glCreateProgram();

		bool result;

		if (mVSData)
		{
			mVS = glCreateShader(GL_VERTEX_SHADER);
			result = Compile(&mVS, GL_VERTEX_SHADER, mVSData, mVSSize);
			LD_DEBUG_ASSERT(result && "GLProgram vertex shader compile error");
			glAttachShader(mProgram, mVS);
		}

		if (mFSData)
		{
			mFS = glCreateShader(GL_FRAGMENT_SHADER);
			result = Compile(&mFS, GL_FRAGMENT_SHADER, mFSData, mFSSize);
			LD_DEBUG_ASSERT(result && "GLProgram fragment shader compile error");
			glAttachShader(mProgram, mFS);
		}

		int linkStatus;
		glLinkProgram(mProgram);
		glGetProgramiv(mProgram, GL_LINK_STATUS, &linkStatus);

		LD_DEBUG_CANARY(linkStatus == GL_TRUE, [&](const char*) {
			char infoLog[512];

			glGetProgramInfoLog(mProgram, sizeof(infoLog), NULL, infoLog);
			std::cout << "GLProgram linkage error\n" << infoLog << std::endl;
		});

		if (mVSData)
			glDeleteShader(mVS);
		if (mFSData)
			glDeleteShader(mFS);

		mContext->BindProgram(*this);
	}

	void GLProgram::Cleanup()
	{
		glDeleteProgram(mProgram);

		mHandle.Reset();
		mContext = nullptr;
	}

	void GLProgram::Bind()
	{
		LD_DEBUG_ASSERT(mContext != nullptr);

		mContext->BindProgram(*this);
	}

	bool GLProgram::SetUniformInt(const std::string& name, int value)
	{
		GLint loc = glGetUniformLocation(mProgram, name.c_str());

		if (loc < 0)
			return false;

		glUniform1i(loc, (GLint)value);
		return true;
	}

	bool GLProgram::SetUniformMat4(const std::string& name, const Mat4& value)
	{
		GLint loc = glGetUniformLocation(mProgram, name.c_str());

		if (loc < 0)
			return false;

		glUniformMatrix4fv(loc, 1, GL_FALSE, static_cast<const GLfloat*>(value.GetData()));
		return true;
	}

	bool GLProgram::Compile(GLuint* shader, GLenum stage, const char* data, uint32_t size)
	{
		LD_DEBUG_ASSERT(shader != nullptr);

		const GLint gl_size(size);
		char infoLog[512];
		int compileStatus;
		bool success = true;

		glShaderSource(*shader, 1, &data, &gl_size);
		glCompileShader(*shader);
		glGetShaderiv(*shader, GL_COMPILE_STATUS, &compileStatus);

		LD_DEBUG_CANARY(compileStatus == GL_TRUE, [&](const char*) {
			glGetShaderInfoLog(*shader, sizeof(infoLog), NULL, infoLog);
			std::cout << "GLProgram compile error at stage " << stage << "\n" << infoLog << std::endl;
			std::cout << "Full Shader Code:\n" << data << std::endl;
			success = false;
		})

		return success;
	}

} // namespace LD
