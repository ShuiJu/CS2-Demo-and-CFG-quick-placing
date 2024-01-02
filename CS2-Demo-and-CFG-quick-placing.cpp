#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <filesystem>
#include <locale>
#include <codecvt>

namespace fs = std::filesystem;

// 将窄字符串转换为宽字符串
std::wstring toWideString(const std::string& narrowStr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(narrowStr);
}

// 询问用户是否覆盖文件
bool askForConfirmation(const fs::path& filePath) {
    std::wcout << L"文件 " << filePath << L" 已存在。" << std::endl;
    std::wcout << L"覆盖已存在文件，请输入Y并回车" << std::endl;
    std::wcout << L"跳过当前文件，请输入N并回车" << std::endl;
    std::wcout << L"是否覆盖？：";
    
    wchar_t response;
    std::wcin >> response;

    return (response == L'y' || response == L'Y');
}

// 复制文件
void copyFiles(const fs::path& sourceFolder, const fs::path& destinationFolder, const std::wstring& fileExtension) {
    try {
        fs::create_directories(destinationFolder);

        for (const auto& entry : fs::directory_iterator(sourceFolder)) {
            if (entry.path().extension() == fileExtension) {
                fs::path destinationPath = destinationFolder / entry.path().filename();

                if (fs::exists(destinationPath)) {
                    if (!askForConfirmation(destinationPath)) {
                        // 用户拒绝覆盖，继续下一个文件
                        continue;
                    }
                }

                fs::copy(entry.path(), destinationPath, fs::copy_options::overwrite_existing);
            }
        }
    } catch (const std::exception& e) {
        std::wcerr << L"错误: " << e.what() << std::endl;
    }
}

int main() {
    // 设置本地化信息为当前环境
    setlocale(LC_ALL, "");  

    // 打印欢迎消息
    std::wcout << L"欢迎使用CS2 Demo和CFG快速放置程序。" << std::endl;
    std::wcout << L"作者：嗜金水狙" << std::endl;
    std::wcout << L"本程序项目链接：https://github.com/ShuiJu/CS2-Demo-and-CFG-quick-placing" << std::endl;

    // 将当前工作目录设置为可执行文件所在的目录
    wchar_t executablePath[MAX_PATH];
    GetModuleFileNameW(NULL, executablePath, MAX_PATH);
    fs::path executableFolder = fs::path(executablePath).parent_path();
    SetCurrentDirectoryW(executableFolder.c_str());

    // 检查 CS2BenchMarkInstaller.exe 所在的文件夹内是否同时缺少 cfg 和 dem 文件
    fs::path cfgFilePath = executableFolder / L"cs2.cfg";
    fs::path demFilePath = executableFolder / L"cs1026.dem";

    if (!fs::exists(cfgFilePath) && !fs::exists(demFilePath)) {
        std::wcerr << L"错误: CS2BenchMarkInstaller.exe 所在的文件夹同时缺少 cfg 和 dem 文件。" << std::endl;
        return 1;
    }

    HWND hwnd = FindWindowW(NULL, L"Counter-Strike 2");
    if (hwnd == NULL) {
        std::wcerr << L"错误: 无法找到正在运行的 Counter-Strike 2 窗口。" << std::endl;
        return 1;
    }

    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle == NULL) {
        std::wcerr << L"错误: 无法打开进程。" << std::endl;
        return 1;
    }

    wchar_t buffer[MAX_PATH];
    if (GetModuleFileNameExW(processHandle, NULL, buffer, MAX_PATH) == 0) {
        std::wcerr << L"错误: 无法获取模块文件名。" << std::endl;
        CloseHandle(processHandle);
        return 1;
    }

    fs::path cs2FolderPath = fs::canonical(buffer).parent_path();
    CloseHandle(processHandle);

    // 修正路径以将 dem 和 cfg 文件复制到目标文件夹
    fs::path targetCsgoFolder = cs2FolderPath.parent_path().parent_path() / L"csgo";
    fs::path targetCfgFolder = targetCsgoFolder / L"cfg";

    // 打印信息
    // std::wcout << L"CS2 文件夹路径: " << cs2FolderPath << std::endl;
    // std::wcout << L"目标 CSGO 文件夹路径: " << targetCsgoFolder << std::endl;

    // 将可执行文件夹中的所有 .dem 文件复制到目标 csgo 文件夹
    copyFiles(executableFolder, targetCsgoFolder, L".dem");

    // 将可执行文件夹中的所有 .cfg 文件复制到目标 cfg 文件夹
    copyFiles(executableFolder, targetCfgFolder, L".cfg");

    std::wcout << L"文件复制完成。" << std::endl;

    return 0;
}
