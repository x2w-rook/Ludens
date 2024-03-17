#pragma once

#include <filesystem>
#include "Core/Header/Include/Types.h"
#include "Core/OS/Include/Memory.h"

namespace LD {

	class Path
	{
	public:
		Path() = default;
		Path(const std::filesystem::path&);
		Path(const std::string& str);
		Path(const char* str);

		inline std::string ToString() const
		{
			return mPath.string();
		}

		explicit operator const std::filesystem::path&() const
		{
			return mPath;
		}

	private:
		std::filesystem::path mPath;
	};

	enum class FileMode
	{
		None = 0,
		Read,
		Write
	};

	class File
	{
	public:
		File();
		File(const File&) = delete;
		~File();

		File& operator=(const File&) = delete;

		static bool Exists(const Path& path);
		
		bool Open(const Path& path, FileMode mode = FileMode::Read);
		void Close();
		void Write(const u8* data, size_t size);

		inline const u8* Data() const { return mData; }
		inline size_t Size() const { return mSize; }

	private:
		size_t mSize = 0;
		u8* mData = nullptr;
		FileMode mMode = FileMode::None;
		Path mWritePath;
	};

	class FileSystem
	{
	public:
		FileSystem();

		Path GetWorkingDirectory();
		bool CreateDirectories(const Path& path);
	private:
	};

} // namespace LD