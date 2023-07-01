#include "FileIo.h"

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#undef NOMINMAX
#endif

#include <filesystem>
#include <locale>
#include <codecvt>
#include <sstream>
#include <stdexcept>


namespace
{
#if defined(_WIN32)
    COMDLG_FILTERSPEC FileTypeFilter[] =
    {
        { L"Outpost 2 Map", L"*.map"},
        { L"Outpost 2 VOL Archive", L"*.vol"}
    };

    const std::string StringFromWString(const std::wstring& str)
    {
        const auto length = WideCharToMultiByte(CP_UTF8, 0, &str.at(0), (int)str.size(), nullptr, 0, nullptr, nullptr);

        if (length <= 0)
        {
            throw std::runtime_error("WideCharToMultiByte() failed.");
        }

        std::string out(length, 0);
        WideCharToMultiByte(CP_UTF8, 0, &str.at(0), (int)str.size(), &out.at(0), length, nullptr, nullptr);
        return out;
    }
#endif

};


FileIo::FileIo(SDL_Window& window) :
    mWindow(window)
{
    SDL_GetWindowWMInfo(&mWindow, &mWmInfo);

#if defined(WIN32)
    wchar_t* path{ nullptr };
    std::ignore = SHGetKnownFolderPath(FOLDERID_ComputerFolder, KF_FLAG_CREATE, nullptr, &path);
    CoTaskMemFree(path);
#endif

    const std::wstring separatorWchar(1, std::filesystem::path::preferred_separator);
    mPathSeparator = StringFromWString(separatorWchar);
}


bool FileIo::filePicked() const
{
    return !mFileName.empty();
}


void FileIo::setDefaultFolder(const std::string& folderPath)
{
    mSavePath = folderPath;
}


void FileIo::setFilter(const size_t index)
{
    mFilterIndex = index;
}


/**
 * \return Returns true if a file name was selected, false otherwise.
 */
bool FileIo::pickSaveFile()
{
    const auto filePicked = showFileDialog(FileOperation::Save);

    if (filePicked)
    {
        extractFileName();
    }

    return filePicked;
}


/**
 * \return Returns true if a file name was selected, false otherwise.
 */
bool FileIo::pickOpenFile()
{
    const auto filePicked = showFileDialog(FileOperation::Open);

    if (filePicked)
    {
        extractFileName();
    }

    return filePicked;
}

bool FileIo::pickFolder()
{
    return showFileDialog(FileOperation::Open, true);
}


void FileIo::extractFileName()
{
    std::size_t location = mFileName.find_last_of("/\\");
    mFileName = mFileName.substr(location + 1);
}


/**
 * There are a few assumptions here regarding the Win32 API's handling of the file
 * pick interface -- creating items, setting parameters and options, etc. are all
 * assumed to not fail. If they fail this will undoubtedly result in other issues.
 *
 * The checks are not present for the sake of brevity. If it's determined that the
 * hresult checks are necessary they can be added as needed.
 *
 * \return Returns true if a file name has been picked. False otherwise.
 */
bool FileIo::showFileDialog(FileOperation operation, bool pickFolder)
{
#if defined(WIN32)
    CLSID fileOperation = (operation == FileOperation::Open ? CLSID_FileOpenDialog : CLSID_FileSaveDialog);

    IFileDialog* fileDialog{ nullptr };
    if (!SUCCEEDED(CoCreateInstance(fileOperation, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fileDialog))))
    {
        throw std::runtime_error("Unable to create file IO dialog");
    }

    IShellItem* defaultFolder{ nullptr };
    SHCreateItemFromParsingName(std::wstring(mSavePath.begin(), mSavePath.end()).c_str(), nullptr, IID_PPV_ARGS(&defaultFolder));
    fileDialog->SetDefaultFolder(defaultFolder);
    defaultFolder->Release();

    DWORD dwFlags{};
    fileDialog->GetOptions(&dwFlags);

    fileDialog->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | (pickFolder ? FOS_PICKFOLDERS : 0x00));
    fileDialog->SetFileTypes(ARRAYSIZE(FileTypeFilter), FileTypeFilter);
    fileDialog->SetFileTypeIndex(mFilterIndex + 1);
    fileDialog->SetDefaultExtension(L"map");

    if (operation == FileOperation::Save && !mFileName.empty())
    {
        fileDialog->SetFileName(std::wstring(mFileName.begin(), mFileName.end()).c_str());
    }

    if (!SUCCEEDED(fileDialog->Show(mWmInfo.info.win.window)))
    {
        fileDialog->Release();
        return false;
    }

    IShellItem* item{ nullptr };
    if (SUCCEEDED(fileDialog->GetResult(&item)))
    {
        PWSTR path{ nullptr };

        if (!SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)))
        {
            throw std::runtime_error("Unable to get file name");
        }

        const auto pathStr = StringFromWString(path);
        CoTaskMemFree(path);

        if (pickFolder)
        {
            mFolderPath = pathStr;
        }
        else
        {
            mFileName = pathStr;
        }

        item->Release();
    }

    fileDialog->Release();
#endif

    return true;
}

