// RunWithDll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <shlwapi.h>
#include <strsafe.h>

#pragma comment(lib, "Shlwapi")

using namespace std;
void CreateChildProcess(LPCTSTR fileName, LPCTSTR childFileName) {
    HANDLE fp;
    unsigned char* fBuffer;
    DWORD fSize, dwSize;
    // 读父文件
    fp = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    fSize = GetFileSize(fp, 0);
    fBuffer = (unsigned char*)VirtualAlloc(NULL, fSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    ReadFile(fp, fBuffer, fSize, &dwSize, NULL);
    CloseHandle(fp);

    // 写子文件
    fp = CreateFile(childFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = WriteFile(fp, fBuffer, fSize, &dwBytesWritten, NULL);
    CloseHandle(fp);

    // 创建子进程
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(STARTUPINFO);
    // 需确保父进程完全退出，子进程才执行
    CreateProcess(childFileName, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
}

int main()
{
    std::cout << "运行一些业务代码!\n";
    //TODO 运行三方库的代码

    TCHAR exeFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileName(GetModuleHandle(NULL), exeFilePath, 100);
    PathRemoveFileSpec(exeFilePath);
    wsprintf(exeFilePath, TEXT("%s\\testDeleteSelf.exe"), exeFilePath);
    wcout << "filePath:" << exeFilePath << endl;
    TCHAR childFilePath[MAX_PATH + 1] = { 0 };
    wsprintf(childFilePath, TEXT("%s:a"), exeFilePath);
    wcout << "childPath:" << childFilePath << endl;
    CreateChildProcess(exeFilePath, childFilePath);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
