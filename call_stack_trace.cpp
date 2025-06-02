#include <iostream>
#include <execinfo.h>  // for backtrace
#include <unistd.h>    // for STDERR_FILENO
#include <cxxabi.h>

/**
 * @ingroup fatal
 *
 * @brief Print a stack trace to \c stderr.
 *
 * This function prints a stack trace to \c stderr, which can be useful
 * for debugging purposes. It is typically called when a fatal error occurs.
 */
void PrintStackTrace() {
    void* callstack[128];  // 存储调用栈地址
    int frames = backtrace(callstack, 128);  // 获取当前调用栈
    char** strs = backtrace_symbols(callstack, frames);  // 转换为可读字符串

    // 打印调用栈
    for (int i = 0; i < frames; ++i) {
        std::cerr << strs[i] << std::endl;
    }

    free(strs);  // 释放内存
}

std::string Demangle(const char* name) {
    int status = 0;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    if (demangled) {
        std::string result(demangled);
        free(demangled);
        return result;
    }
    return name;
}

void PrintStackTrace2() {
    void* callstack[128];
    int frames = backtrace(callstack, 128);          // 获取调用栈地址
    char** strs = backtrace_symbols(callstack, frames); // 转换为字符串

    // 打印解析后的调用栈
    for (int i = 0; i < frames; ++i) {
        std::string symbol(strs[i]);
        // 提取函数名（格式：./executable(function+offset) [address]）
        size_t start = symbol.find('(');
        size_t end = symbol.find('+');
        if (start != std::string::npos && end != std::string::npos) {
            std::string func_name = symbol.substr(start + 1, end - start - 1);
            std::cout << i << ": " << Demangle(func_name.c_str()) << std::endl;
        } else {
            std::cout << i << ": " << symbol << std::endl;
        }
    }

    free(strs);  // 释放内存
}

void func0() {
    std::cout << "\n" << std::endl;

    std::cout << "################ Basic Call Stack ####################" << std::endl;
    PrintStackTrace();  // 调用栈追踪

    std::cout << "\n" << std::endl;

    std::cout << "################ Enhanced Call Stack ####################" << std::endl;
    PrintStackTrace2();  // 调用栈追踪
}

void func1() {
    func0();  // 嵌套调用
}

void func2() {
    func1();  // 嵌套调用
}

int main() {
    func2();  // 最终调用
    return 0;
}
