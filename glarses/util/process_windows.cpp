#ifdef WIN32

#include "process.h"
#include "../dependencies.h"
#include "../log/logger.h"
#include <array>

namespace {
    struct PipeHandles {
        PipeHandles() {
            // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa379560(v=vs.85)
            SECURITY_ATTRIBUTES pipe_attributes = {
                    sizeof(SECURITY_ATTRIBUTES),
                    nullptr,
                    TRUE
            };

            if (!CreatePipe(
                    &m_ReadHandle,
                    &m_WriteHandle,
                    &pipe_attributes,
                    0
            ))
                throw std::runtime_error("Failed to create pipe");
        }

        ~PipeHandles() {
            CloseHandle(m_ReadHandle);
            CloseHandle(m_WriteHandle);
        }

        PipeHandles             (const PipeHandles&) = delete;
        PipeHandles& operator = (const PipeHandles&) = delete;

        HANDLE m_ReadHandle  = nullptr;
        HANDLE m_WriteHandle = nullptr;
    };

    struct Win32Process {
        Win32Process(
                const std::filesystem::path& working_directory,
                const std::filesystem::path& full_executable_path,
                const char*                  arguments,
                PipeHandles&                 std_in,
                PipeHandles&                 std_out
        ) {
            // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa
            STARTUPINFOA startup = {};
            startup.cb          = sizeof(STARTUPINFOA);
            startup.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
            startup.wShowWindow = SW_SHOW;
            startup.hStdInput   = std_in.m_ReadHandle;
            startup.hStdOutput  = std_out.m_WriteHandle;
            startup.hStdError   = std_out.m_WriteHandle;

            // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
            if (!CreateProcessA(
                    full_executable_path.string().c_str(),  // application name
                    (char*)arguments,                       // commandline arguments
                    nullptr,                                // process attributes
                    nullptr,                                // thread attributes
                    TRUE,                                   // inherit handles
                    0,                                      // creation flags
                    nullptr,                                // environment
                    working_directory.string().c_str(),     // working directory
                    &startup,                               // startup info
                    &m_Info                                 // process information
            )) {
                // try and grab the error message
                // NOTE I used to have something for this, should search old projects
                DWORD code         = GetLastError();
                char* local_buffer = nullptr;

                if (!FormatMessageA(
                        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, // flags
                        nullptr,                                                     // source
                        code,                                                        // message id
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                   // language id
                        local_buffer,                                                // output buffer
                        0,                                                           // output buffer size
                        nullptr                                                      // variadic arguments
                ))
                    throw std::runtime_error("Failed to format error message");

                g_LogError << "Failed to create process: " << std::string(local_buffer);

                LocalFree(local_buffer); // egh this isn't quite right

                throw std::runtime_error("Failed to create process");
            }

            // read back the process output to stdout
            // (derived from https://learn.microsoft.com/en-us/windows/win32/ipc/named-pipe-client)
            DWORD num_bytes_read = 0;

            static constexpr size_t                     k_ProcessOutputBufferSize = 1024;
            std::array<char, k_ProcessOutputBufferSize> process_output_buffer     = {};

            BOOL success = FALSE;

            do {
                success = ReadFile(
                        std_out.m_ReadHandle,                             // source handle
                        process_output_buffer.data(),                     // dst buffer
                        static_cast<DWORD>(process_output_buffer.size()), // dst buffer size
                        &num_bytes_read,                                  // number of bytes read
                        nullptr                                           // overlapped I/O struct
                );

                if (!success && GetLastError() != ERROR_MORE_DATA)
                    break;

                m_ProcessOutput += std::string(process_output_buffer.data(), num_bytes_read);
            } while (!success);

            // get back the exit code
            DWORD process_exit_code = 0;
            GetExitCodeProcess(m_Info.hProcess, &process_exit_code);
            m_ExitCode = process_exit_code;
        }

        ~Win32Process() {
            CloseHandle(m_Info.hThread);
            CloseHandle(m_Info.hProcess);
        }

        Win32Process             (const Win32Process&) = delete;
        Win32Process& operator = (const Win32Process&) = delete;

        PROCESS_INFORMATION m_Info = {};

        unsigned long m_ExitCode = 0;
        std::string   m_ProcessOutput;
    };
}

namespace glarses {
    std::optional<ProcessOutput> execute_process(
            const std::filesystem::path& working_directory,
            const std::filesystem::path& full_executable_path,
            const char*                  arguments
    )  {
        ProcessOutput result;

        // we'll redirect stdin/stdout so we can read back output
        // for now we'll also just show the process window
        try {
            PipeHandles std_in;
            PipeHandles std_out;

            // this feels like it should be a function object instead
            // should refactor at some point
            Win32Process proc(
                    working_directory,
                    full_executable_path,
                    arguments,
                    std_in,
                    std_out
            );

            result.m_ExitCode = proc.m_ExitCode;
            result.m_Output   = std::move(proc.m_ProcessOutput);
        }
        catch (std::exception& ex) {
            g_LogError << "Exception: " << ex.what();
            return std::nullopt;
        }

        return result;
    }
}

#endif