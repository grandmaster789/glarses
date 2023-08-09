#include "crash_handler.h"

// this entire thing is very specific to windows
//
#ifdef WIN32

#include "../dependencies.h"
#include "../log/logger.h"
#include "../util/defer.h"

#include <string>

// stack trace
// (based around https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-stackwalk64)
namespace {
    void capture_stack_trace(
            CONTEXT*               ctx,
            std::vector<uint64_t>& frame_pointers
    ) {
        DWORD        machine_type = 0x0;
        STACKFRAME64 frame        = {};

        frame.AddrPC.Mode    = AddrModeFlat;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Mode = AddrModeFlat;

#if defined(_M_X64)
        frame.AddrPC.Offset    = ctx->Rip;
        frame.AddrFrame.Offset = ctx->Rbp;
        frame.AddrStack.Offset = ctx->Rsp;

        machine_type = IMAGE_FILE_MACHINE_AMD64;
#else
        // assume x86

        frame.AddrPC.Offset    = ctx->Eip;
        frame.AddrFrame.Offset = ctx->Ebp;
        frame.AddrStack.Offset = ctx->Esp;

        machine_type = IMAGE_FILE_MACHINE_I386;
#endif

        auto current_process = GetCurrentProcess();
        auto current_thread  = GetCurrentThread();

        for (auto& ptr: frame_pointers) {
            if (StackWalk64(
                    machine_type,
                    current_process,
                    current_thread,
                    &frame,
                    ctx,
                    nullptr,
                    SymFunctionTableAccess64,
                    SymGetModuleBase64,
                    nullptr
            ))
                ptr = frame.AddrPC.Offset;
            else
                break;
        }
    }

    std::string get_symbol_info(
            size_t                       index,
            const std::vector<uint64_t>& frame_pointers
    ) {
        auto addr   = frame_pointers[index];
        auto result = "[" + std::to_string(index) + "]\t";

        // some trickery to get around the 'flexible' struct
        alignas(SYMBOL_INFO) char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
        auto* info = reinterpret_cast<SYMBOL_INFO*>(buffer);

        info->SizeOfStruct = sizeof(SYMBOL_INFO);
        info->MaxNameLen   = MAX_SYM_NAME;

        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        std::string line_info;
        std::string call_info;

        DWORD64 d64;
        DWORD   displacement;

        auto process = GetCurrentProcess();

        if (SymFromAddr(
                process,
                addr,
                &d64,
                info
        )) {
            call_info
                    .append(" ")
                    .append(std::string(info->Name, info->NameLen));

            if (SymGetLineFromAddr64(
                    process,
                    addr,
                    &displacement,
                    &line
            )) {
                line_info
                        .append("\t")
                        .append(line.FileName)
                        .append(":")
                        .append(std::to_string(line.LineNumber));
            }
        }

        result
                .append(line_info)
                .append(call_info);

        return result;
    }

    std::string frames_to_text(const std::vector<uint64_t>& frame_pointers) {
        std::string result;

        for (size_t index = 0; index < frame_pointers.size(); ++index) {
            if (frame_pointers[index]) {
                result += get_symbol_info(index, frame_pointers);
                result += '\n';
            }
        }

        return result;
    }

    // NOTE this is kind of questionable; it's good if we're always just using this
    //      in crash scenarios, but it's bad if we can actually recover
    thread_local size_t g_RecursiveGuard = 0;

    std::string make_stack_trace(CONTEXT* ctx) {
        if (g_RecursiveGuard >= 2)
            return "\n\n<##! Error !##> Recursive crash detected\n";

        ++g_RecursiveGuard;

        static std::mutex mut;

        std::lock_guard guard(mut);

        {
            // https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-symsetoptions
            SymSetOptions(
                    SYMOPT_UNDNAME        |
                    SYMOPT_DEFERRED_LOADS
            );

            auto initialized = SymInitialize(
                    GetCurrentProcess(),
                    nullptr,
                    TRUE                 // invade process
            );

            auto x = glarses::defer([] {
                SymCleanup(GetCurrentProcess());
            });

            if (!initialized)
                return "Cannot retrieve stack symbols";

            constexpr size_t k_Max = 64;

            std::vector<uint64_t> frame_pointers(k_Max);
            capture_stack_trace(ctx, frame_pointers);

            return frames_to_text(frame_pointers);
        }
    }

    std::string make_stack_trace(EXCEPTION_POINTERS* info) {
        auto ctx = info->ContextRecord;
        return make_stack_trace(ctx);
    }

    // create stack trace without exception pointer
    std::string make_stack_trace() {
        CONTEXT ctx = {};

        RtlCaptureContext(&ctx);

        return make_stack_trace(&ctx);
    }
}

// dump writer
namespace {
    //
    // helper around win32 api MiniDumpWriteDump
    //
    class DumpWriter {
    private:
        DumpWriter() = default;

    public:
        static DumpWriter& instance() {
            static DumpWriter x;
            return x;
        }

        static DWORD CALLBACK writer(LPVOID /*params*/) {
            // auto* self = (DumpWriter*)params; // if some class variable is needed


            // figure out the filename of the running executable
            wchar_t module_filename[MAX_PATH + 1] = {};
            GetModuleFileNameW(nullptr, module_filename, MAX_PATH + 1);
            std::wstring module_string = module_filename;

            // get rid of the '.exe'
            auto dump_path = module_string.substr(0, module_string.size() - 4);

            // append date-time formatting '[YMD HMS]'
            {
                auto now = std::chrono::system_clock::now();
                std::wostringstream wss;
                wss << std::format(L"[{:%Y%m%d %H%M%OS}]", now);
                dump_path += wss.str();
            }

            // append '.dmp'
            dump_path += L".dmp";

            HANDLE dump_file = CreateFileW(
                    dump_path.c_str(),
                    GENERIC_WRITE,
                    0,                     // share mode
                    nullptr,               // security attribute
                    CREATE_ALWAYS,         // create disposition
                    FILE_ATTRIBUTE_NORMAL, // flags and attributes
                    nullptr                // template file
            );

            if (dump_file == INVALID_HANDLE_VALUE) {
                MessageBoxW(
                        nullptr,
                        L"Failed to create dumpfile",
                        L"glarses_dump",
                        MB_OK
                );
                return 1;
            }

            // https://learn.microsoft.com/en-us/windows/win32/api/minidumpapiset/ne-minidumpapiset-minidump_type
            static constexpr MINIDUMP_TYPE dump_type_mask = MINIDUMP_TYPE::MiniDumpNormal;

            // https://learn.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
            MiniDumpWriteDump(
                    GetCurrentProcess(),
                    GetCurrentProcessId(),
                    dump_file,
                    dump_type_mask,
                    nullptr, // exception params
                    nullptr, // user streams
                    nullptr  // callback
            );

            CloseHandle(dump_file);

            return 0;
        }

        void create_dump() {
            DWORD  writer_thread_id = 0x0;

            // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
            HANDLE writer_thread = CreateThread(
                    nullptr,           // thread attributes
                    0,                 // stack size
                    writer,            // thread routine
                    this,              // user params
                    CREATE_SUSPENDED,  // creation flags
                    &writer_thread_id  // resulting thread id
            );

            if (writer_thread) {
                // suspend all threads except this one
                applyToThreads(SuspendThread, writer_thread_id);

                // start the writer thread, wait for it to complete
                ResumeThread(writer_thread);
                WaitForSingleObject(writer_thread, INFINITE);

                DWORD thread_exit_code = 0;
                GetExitCodeThread(writer_thread, &thread_exit_code);
                CloseHandle(writer_thread);

                // resume all threads
                applyToThreads(ResumeThread, writer_thread_id);

                if (thread_exit_code)
                    MessageBoxW(
                            nullptr,
                            L"Crash dump writer thread failed",
                            L"glarses_dump",
                            MB_OK
                    );
            }
            else
                MessageBoxW(
                        nullptr,
                        L"Failed to create dump writer thread",
                        L"glarses_dump",
                        MB_OK
                );
        }

    private:
        void applyToThreads(
                DWORD(WINAPI* callbackFn)(HANDLE),
                DWORD         ignore_this_thread_handle
        ) const {
            // create a snapshot of all other threads in the process, except for the thread
            // identified by the ignore_this_thread_handle parameter

            // https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

            if (snapshot != INVALID_HANDLE_VALUE) {
                THREADENTRY32 entry;
                entry.dwSize = sizeof(entry);

                if (Thread32First(snapshot, &entry)) {
                    const auto this_process_id = GetCurrentProcessId();
                    const auto this_thread_id  = GetCurrentThreadId();

                    do {
                        if (entry.th32OwnerProcessID == this_process_id &&
                            entry.th32ThreadID != ignore_this_thread_handle &&
                            entry.th32ThreadID != this_thread_id
                        ) {
                            HANDLE thread = OpenThread(
                                    THREAD_SUSPEND_RESUME, // desired access
                                    FALSE,                 // inherit handle
                                    entry.th32ThreadID
                            );

                            if (thread) {
                                callbackFn(thread);
                                CloseHandle(thread);
                            }
                        }
                    } while (Thread32Next(snapshot, &entry));
                }
            }
        }
    };

    LONG WINAPI crash_handler(LPEXCEPTION_POINTERS exception) {
        // log a stack trace for the current thread
        g_LogError << '\n' << make_stack_trace(exception);

        DumpWriter::instance().create_dump();

        return EXCEPTION_CONTINUE_SEARCH; // pass the exception to the OS
    }

    void install_crash_handler_all_exceptions() {
        // https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-addvectoredexceptionhandler
        AddVectoredExceptionHandler(
                1, // the crash handler should be the first to be called
                crash_handler
        );
    }

    void install_crash_handler_unhandled_exceptions() {
        // NOTE this seems to fail in some circumstances
        SetUnhandledExceptionFilter(crash_handler);
    }
}



namespace glarses::os {
    void install_crash_handler() {
        // by default, make a stacktrace error message, log it and create a dumpfile
        //

        // signal handlers (SIGFPE, SIGABRT etc) can cause deadlocks, just leave them alone

        // install_crash_handler_unhandled_exceptions();
        install_crash_handler_all_exceptions(); // this may lead to false positives, especially when exceptions are used as control flow
    }
}

#endif