#include <Windows.h>
#include <shlwapi.h>
#include <strsafe.h>

#pragma comment(lib, "Shlwapi")

// 删除当前目录

void DeleteDirectoryR(LPCTSTR lpszDir)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    TCHAR DirPath[MAX_PATH+1];
    TCHAR FileName[MAX_PATH+1];

    StringCchPrintf(DirPath, MAX_PATH, TEXT("%s\\*"), lpszDir);

    hFind = FindFirstFile(DirPath, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    StringCchPrintf(FileName, MAX_PATH, TEXT("%s\\"), lpszDir);

    do
    {
        if (StrCmpI(FindFileData.cFileName, TEXT(".")) == 0 || StrCmpI(FindFileData.cFileName, TEXT("..")) == 0)
            continue;

        StringCbCat(FileName, MAX_PATH, FindFileData.cFileName);

        if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            DeleteDirectoryR(FileName);
            RemoveDirectory(FileName);
        }
        else
        {
            DeleteFile(FileName);
        }
        StringCchPrintf(FileName, MAX_PATH, TEXT("%s\\"), lpszDir);
    } while (FindNextFile(hFind, &FindFileData));

    FindClose(hFind);
    RemoveDirectory(FileName);
}

BOOL IsExistPath(LPCTSTR path)
{
    DWORD fa = INVALID_FILE_ATTRIBUTES;
    SetLastError(ERROR_SUCCESS);
    fa = GetFileAttributesW(path);
    if (fa == INVALID_FILE_ATTRIBUTES)
    {
        DWORD error = GetLastError();
        return error != ERROR_PATH_NOT_FOUND &&
            error != ERROR_FILE_NOT_FOUND;
    }
    else
    {
        // 获取文件属性成功，路径存在
        return TRUE;
    }
}


int main()
{
    TCHAR szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    TCHAR szDirPath[MAX_PATH + 1] = { 0 };
    StringCbCopy(szDirPath, MAX_PATH, szFilePath);
    PathRemoveFileSpec(szDirPath);
    do 
    {
        DeleteDirectoryR(szDirPath);
        if (!IsExistPath(szDirPath)) {
            break;
        }
        Sleep(100);
    } while (1);
    return 0;
}
