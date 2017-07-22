////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
// Copyright (C) 2017 Nicolas Kogler
//
// Cranberry is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Cranberry is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Cranberry. If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////


// Cranberry headers
#include <Cranberry/System/StackWalkers/StackWalkerWin32.hpp>

// Qt headers
#include <QStringList>

// Platform headers
#include <Windows.h>
#include <DbgHelp.h>

// Standard headers
#include <exception>

// Constants
CRANBERRY_CONST_VAR(QString, c_msg_frame, "[%0] %1 in %2 at line %3")
CRANBERRY_CONST_VAR(uint, c_max_frames, 30)


CRANBERRY_USING_NAMESPACE


QString priv::StackWalkerWin32::stackTrace()
{
    QStringList trace;

    try
    {
        // Set up process, thread and context.
        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();
        CONTEXT context;

        std::memset(&context, 0, sizeof(CONTEXT));
        context.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&context);
        SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES);
        SymInitialize(process, NULL, TRUE);

        // Set up the machine-dependent STACKFRAME64 structure.
        DWORD image;
        STACKFRAME64 frame;
        ZeroMemory(&frame, sizeof(STACKFRAME64));

    #ifdef _M_IX86
        image = IMAGE_FILE_MACHINE_I386;
        frame.AddrPC.Offset = context.Eip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Ebp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Esp;
        frame.AddrStack.Mode = AddrModeFlat;
    #elif _M_X64
        image = IMAGE_FILE_MACHINE_AMD64;
        frame.AddrPC.Offset = context.Rip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Rsp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Rsp;
        frame.AddrStack.Mode = AddrModeFlat;
    #elif _M_IA64
        frame.AddrPC.Offset = context.StIIP;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.IntSp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrBStore.Offset = context.RsBSP;
        frame.AddrBStore.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.IntSp;
        frame.AddrStack.Mode = AddrModeFlat;
    #endif

        // Walks the stack for at max. 30 iterations.
        for (size_t i= 0; i < c_max_frames; i++)
        {
            // Tries to walk the stack.
            if (!StackWalk64(
                image, process, thread,
                &frame, &context, NULL,
                SymFunctionTableAccess64,
                SymGetModuleBase64, NULL
                ))
            {
                break;
            }

            // Initializes a new PSYMBOL_INFO and IMAGEHLP_LINE structure.
            CHAR buffer1[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
            CHAR buffer2[sizeof(IMAGEHLP_LINE)];
            PSYMBOL_INFO symbol = (PSYMBOL_INFO) buffer1;
            IMAGEHLP_LINE* line = (IMAGEHLP_LINE*) buffer2;

            // Extracts all the information.
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            symbol->MaxNameLen = MAX_SYM_NAME;

            DWORD64 dsp = 0;
            DWORD dspl = 0;
            if (SymFromAddr(process, frame.AddrPC.Offset, &dsp, symbol))
            {
                if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &dspl, line))
                {
                    trace.append(c_msg_frame.arg(
                        QString::number(i),
                        symbol->Name,
                        line->FileName,
                        QString::number(line->LineNumber)
                        ));
                }
                else
                {
                    trace.append(c_msg_frame.arg(
                        QString::number(i),
                        symbol->Name,
                        "???",
                        "???"
                        ));
                }
            }
            else
            {
                trace.append(c_msg_frame.arg(QString::number(i), "???", "???", "???"));
            }
        }

        SymCleanup(process);

        return trace.join('\n');
    }
    catch (std::exception& e)
    {
        trace.append(e.what());
        return trace.join('\n');
    }
}
