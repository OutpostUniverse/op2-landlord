#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <string>


class FileIo
{
public:
	FileIo() = delete;
	FileIo(const FileIo&) = delete;
	const FileIo& operator=(const FileIo&) = delete;

	FileIo(SDL_Window& window);
	~FileIo() = default;

	const std::string& savePath() const { return mSavePath; }
	const std::string& fileName() const { return mFileName; }
	const std::string& fullPath() const { return mSavePath + "\\" + mFileName; }
	const std::string& folderPath() const { return mFolderPath; }
	const std::string& pathSeparator() const { return mPathSeparator; }

	void setDefaultFolder(const std::string& folderPath);

	void setFilter(const size_t index);

	bool pickSaveFile();
	bool pickOpenFile();

	bool pickFolder();

	bool filePicked() const;

private:
	enum class FileOperation { Open, Save };

	bool showFileDialog(FileOperation, bool pickFolder = false);
	void extractFileName();

	SDL_Window& mWindow;
	SDL_SysWMinfo mWmInfo{};

	std::string mSavePath;
	std::string mFileName;
	std::string mFolderPath;
	std::string mPathSeparator;
	size_t mFilterIndex = 0;
};
