#include "fmtdbghelp.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#include <Dbghelp.h>

FMTDBGHELPSHARED_EXPORT FmtDbgHelp *CreateDbgHelpObject(QObject *parent)
{
    FmtDbgHelp *obj = new FmtDbgHelp(parent);
    return obj;
}

FmtDbgHelp::FmtDbgHelp(QObject *parent) :
    QObject(parent)
{
    hProcess = GetCurrentProcess();
    SymInitialize(hProcess, NULL, true);

    m_ExceptionCodes[EXCEPTION_ACCESS_VIOLATION] = "EXCEPTION_ACCESS_VIOLATION";
    m_ExceptionCodes[EXCEPTION_ARRAY_BOUNDS_EXCEEDED] = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
    m_ExceptionCodes[EXCEPTION_BREAKPOINT] = "EXCEPTION_BREAKPOINT";
    m_ExceptionCodes[EXCEPTION_DATATYPE_MISALIGNMENT] = "EXCEPTION_DATATYPE_MISALIGNMENT";
    m_ExceptionCodes[EXCEPTION_FLT_DENORMAL_OPERAND] = "EXCEPTION_FLT_DENORMAL_OPERAND";
    m_ExceptionCodes[EXCEPTION_FLT_DIVIDE_BY_ZERO] = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
    m_ExceptionCodes[EXCEPTION_FLT_INEXACT_RESULT] = "EXCEPTION_FLT_INEXACT_RESULT";
    m_ExceptionCodes[EXCEPTION_FLT_INVALID_OPERATION] = "EXCEPTION_FLT_INVALID_OPERATION";
    m_ExceptionCodes[EXCEPTION_FLT_OVERFLOW] = "EXCEPTION_FLT_OVERFLOW";
    m_ExceptionCodes[EXCEPTION_FLT_STACK_CHECK] = "EXCEPTION_FLT_STACK_CHECK";
    m_ExceptionCodes[EXCEPTION_FLT_UNDERFLOW] = "EXCEPTION_FLT_UNDERFLOW";
    m_ExceptionCodes[EXCEPTION_ILLEGAL_INSTRUCTION] = "EXCEPTION_ILLEGAL_INSTRUCTION";
    m_ExceptionCodes[EXCEPTION_IN_PAGE_ERROR] = "EXCEPTION_IN_PAGE_ERROR";
    m_ExceptionCodes[EXCEPTION_INT_DIVIDE_BY_ZERO] = "EXCEPTION_INT_DIVIDE_BY_ZERO";
    m_ExceptionCodes[EXCEPTION_INT_OVERFLOW] = "EXCEPTION_INT_OVERFLOW";
    m_ExceptionCodes[EXCEPTION_INVALID_DISPOSITION] = "EXCEPTION_INVALID_DISPOSITION";
    m_ExceptionCodes[EXCEPTION_NONCONTINUABLE_EXCEPTION] = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
    m_ExceptionCodes[EXCEPTION_PRIV_INSTRUCTION] = "EXCEPTION_PRIV_INSTRUCTION";
    m_ExceptionCodes[EXCEPTION_SINGLE_STEP] = "EXCEPTION_SINGLE_STEP";
    m_ExceptionCodes[EXCEPTION_STACK_OVERFLOW] = "EXCEPTION_STACK_OVERFLOW";

    m_ExceptionText[EXCEPTION_ACCESS_VIOLATION] = "The thread tried to read from or write to a virtual address for which it does not have the appropriate access.";
    m_ExceptionText[EXCEPTION_ARRAY_BOUNDS_EXCEEDED] = "The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.";
    m_ExceptionText[EXCEPTION_BREAKPOINT] = "A breakpoint was encountered.";
    m_ExceptionText[EXCEPTION_DATATYPE_MISALIGNMENT] = "The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.";
    m_ExceptionText[EXCEPTION_FLT_DENORMAL_OPERAND] = "One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.";
    m_ExceptionText[EXCEPTION_FLT_DIVIDE_BY_ZERO] = "The thread tried to divide a floating-point value by a floating-point divisor of zero.";
    m_ExceptionText[EXCEPTION_FLT_INEXACT_RESULT] = "The result of a floating-point operation cannot be represented exactly as a decimal fraction.";
    m_ExceptionText[EXCEPTION_FLT_INVALID_OPERATION] = "This exception represents any floating-point exception not included in this list.";
    m_ExceptionText[EXCEPTION_FLT_OVERFLOW] = "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.";
    m_ExceptionText[EXCEPTION_FLT_STACK_CHECK] = "The stack overflowed or underflowed as the result of a floating-point operation.";
    m_ExceptionText[EXCEPTION_FLT_UNDERFLOW] = "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.";
    m_ExceptionText[EXCEPTION_ILLEGAL_INSTRUCTION] = "The thread tried to execute an invalid instruction.";
    m_ExceptionText[EXCEPTION_IN_PAGE_ERROR] = "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.";
    m_ExceptionText[EXCEPTION_INT_DIVIDE_BY_ZERO] = "The thread tried to divide an integer value by an integer divisor of zero.";
    m_ExceptionText[EXCEPTION_INT_OVERFLOW] = "The result of an integer operation caused a carry out of the most significant bit of the result.";
    m_ExceptionText[EXCEPTION_INVALID_DISPOSITION] = "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.";
    m_ExceptionText[EXCEPTION_NONCONTINUABLE_EXCEPTION] = "The thread tried to continue execution after a noncontinuable exception occurred.";
    m_ExceptionText[EXCEPTION_PRIV_INSTRUCTION] = "The thread tried to execute an instruction whose operation is not allowed in the current machine mode.";
    m_ExceptionText[EXCEPTION_SINGLE_STEP] = "A trace trap or other single-instruction mechanism signaled that one instruction has been executed.";
    m_ExceptionText[EXCEPTION_STACK_OVERFLOW] = "The thread used up its stack.";
}

FmtDbgHelp::~FmtDbgHelp()
{
    SymCleanup(hProcess);
}

bool FmtDbgHelp::WritMiniDump(const QString &dumpFileName, void *pExInfo)
{
    bool hr = true;
    HANDLE hFile = NULL;
    hFile = CreateFileA(dumpFileName.toLocal8Bit().data(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    //qDebug() << dumpFileName.toLocal8Bit().data();
    if(NULL == hFile || INVALID_HANDLE_VALUE == hFile)
        hr =  false;

    PEXCEPTION_POINTERS ExInfo = (PEXCEPTION_POINTERS)pExInfo;
    if (hr)
    {
        MINIDUMP_EXCEPTION_INFORMATION eInfo;
        memset(&eInfo, 0, sizeof(MINIDUMP_EXCEPTION_INFORMATION));
        eInfo.ThreadId = GetCurrentThreadId();
        eInfo.ExceptionPointers = (PEXCEPTION_POINTERS)pExInfo;
        eInfo.ClientPointers = FALSE;

        const DWORD Flags = MiniDumpWithDataSegs |
                MiniDumpWithPrivateReadWriteMemory |
                MiniDumpWithHandleData |
                MiniDumpWithFullMemory |
                MiniDumpWithFullMemoryInfo |
                MiniDumpWithThreadInfo |
                MiniDumpWithUnloadedModules |
                MiniDumpWithProcessThreadData;

        hr = MiniDumpWriteDump(hProcess, GetCurrentProcessId(), hFile, (MINIDUMP_TYPE)Flags, &eInfo, NULL, NULL);
        CloseHandle(hFile);
    }

    /*if (hr)
    {
        DWORD64  dwDisplacement = 0;
        DWORD64  dwAddress = (DWORD64)ExInfo->ExceptionRecord->ExceptionAddress;

        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;

        if (SymFromAddr(hProcess, dwAddress, &dwDisplacement, pSymbol))
        {
            // SymFromAddr returned success
            qDebug() << pSymbol->Name;
        }
        else
        {
            DWORD error = GetLastError();
            qDebug() << "SymFromAddr returned error:" << error;
        }
    }*/

    return hr;
}

void FmtDbgHelp::GetExceptionInfo(void *pExInfo, QString &module, QString &function, QString &file, int &line)
{
   PEXCEPTION_POINTERS ExInfo = (PEXCEPTION_POINTERS)pExInfo;
   char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
   PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
   pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   pSymbol->MaxNameLen = MAX_SYM_NAME;

   DWORD dwDisplacement = 0;
   if (!GetSymbolNameByAddrPrivate((DWORD64)ExInfo->ExceptionRecord->ExceptionAddress, pSymbol))
   {
       IMAGEHLP_LINE64 sLine;
       memset(&sLine, 0, sizeof(IMAGEHLP_LINE64));
       if (SymGetLineFromAddr64(hProcess, (DWORD64)ExInfo->ExceptionRecord->ExceptionAddress, &dwDisplacement, &sLine))
       {
           file = sLine.FileName;
           line = sLine.LineNumber;
       }

       if (strlen(pSymbol->Name))
           function = pSymbol->Name;
       else
           function = QString("0x%1").arg(pSymbol->Address, 8, 16, QLatin1Char('0'));

       IMAGEHLP_MODULE64 ImageModule;
       memset(&ImageModule, 0, sizeof(IMAGEHLP_MODULE64));
       ImageModule.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

       if (SymGetModuleInfo64(hProcess, (DWORD64)pSymbol->Address, &ImageModule))
           module = ImageModule.ImageName;
   }
}

int FmtDbgHelp::GetSymbolNameByAddrPrivate(DWORD64 ExceptionAddress, void *pSymbol)
{
    int stat = 0;
    DWORD64 dwDisplacement = 0;
    if (!SymFromAddr(hProcess, (DWORD64)ExceptionAddress, &dwDisplacement, (PSYMBOL_INFO)pSymbol))
    {
        stat = GetLastError();
        qDebug() << stat;
    }
    return stat;
}

QString FmtDbgHelp::ExceptionCodeShort(const int &ExceptionCode)
{
    return m_ExceptionCodes[ExceptionCode];
}

QString FmtDbgHelp::ExceptionCodeText(const int &ExceptionCode)
{
    return m_ExceptionText[ExceptionCode];
}

/*void FmtDbgHelp::RetrieveCallStack(QStringList *stackList)
{
    STACKFRAME64 stack;
    memset(&stack, 0, sizeof(STACKFRAME64));

    unsigned __int64 dwDisplacement = 0;
    HANDLE hThread = GetCurrentThread();
    CONTEXT context;
    RtlCaptureContext(&context);
    //GetThreadContext(hThread, &threadContext);

    const int NAME_SIZE = 256;
    QScopedPointer<IMAGEHLP_SYMBOL64> Symbol(reinterpret_cast<PIMAGEHLP_SYMBOL64>(new char[sizeof(IMAGEHLP_SYMBOL64) + NAME_SIZE]));

    stack.AddrPC.Offset    = context.Eip;
    stack.AddrPC.Mode      = AddrModeFlat;
    stack.AddrStack.Offset = context.Esp;
    stack.AddrStack.Mode   = AddrModeFlat;
    stack.AddrFrame.Offset = context.Ebp;
    stack.AddrFrame.Mode   = AddrModeFlat;

    while(StackWalk64(IMAGE_FILE_MACHINE_I386, hProcess, hThread, &stack,
                   &context, NULL, SymFunctionTableAccess64,
                   SymGetModuleBase64, 0))
    {
        QString callStackString;
        IMAGEHLP_MODULE64 ImageModule;
        memset(&ImageModule, 0, sizeof(IMAGEHLP_MODULE64));
        ImageModule.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

        QString moduleName;
        if (SymGetModuleInfo64(hProcess, stack.AddrPC.Offset, &ImageModule))
            moduleName = ImageModule.ModuleName;

        memset(Symbol.data(), 0, sizeof(IMAGEHLP_SYMBOL64));

        if (SymGetSymFromAddr64(hProcess, (ULONG64)stack.AddrPC.Offset, &dwDisplacement, Symbol.data()))
        {
            callStackString = QString("%1!%2")
                    .arg(moduleName)
                    .arg(Symbol->Name);
            stackList->append(callStackString);
        }
    }
}*/

#endif
