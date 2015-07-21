// homework1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "util.h"

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
void print(_In_ const char* fmt, _In_ ...)
{
  char log_buffer[2048];
  va_list args;

  va_start(args, fmt);
  HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
  if (S_OK != hRes)
  {
    fprintf(
      stderr,
      "%s, StringCbVPrintfA() failed. res = 0x%08x",
      __FUNCTION__,
      hRes
      );
    return;
  }

  OutputDebugStringA(log_buffer);
  fprintf(stdout, "%s \n", log_buffer);
}

bool is_file_existsW(_In_ const wchar_t* file_path)
{
  _ASSERTE(NULL != file_path);
  _ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
  if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

  WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

  if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info) == 0)
    return false;
  else
    return true;
}

wchar_t* MbsToWcs(_In_ const char* mbs)
{
    _ASSERTE(NULL!=mbs);
    if(NULL==mbs) return NULL;

    int outLen=MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, mbs, -1, NULL, 0);
    if(0==outLen) return NULL;

    wchar_t* outWchar=(wchar_t*) malloc(outLen * (sizeof(wchar_t)));  // outLen contains NULL char
    if(NULL==outWchar) return NULL;
    RtlZeroMemory(outWchar, outLen);

  if (0 == MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, mbs, -1, outWchar, outLen))
  {
        free(outWchar);
        return NULL;
    }

    return outWchar;
}

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool create_bob_txt()
{
  // current directory ¸¦ ±¸ÇÑ´Ù.
  wchar_t *buf = NULL;
  uint32_t buflen = 0;
  buflen = GetCurrentDirectoryW(buflen, buf);
  if (0 == buflen)
  {
    print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
    return false;
  }

  buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
  if (0 == GetCurrentDirectoryW(buflen, buf))
  {
    print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
    free(buf);
    return false;
  }

  // current dir \\ bob.txt ÆÄÀÏ¸í »ý¼º
  wchar_t file_name[260];
  wchar_t file_name_bob2[260];
  if (!SUCCEEDED(StringCbPrintfW(
    file_name,
    sizeof(file_name),
    L"%ws\\bob.txt",
    buf)))
  {
    print("err, can not create file name");
    free(buf);
    return false;
  }
  if (!SUCCEEDED(StringCbPrintfW(
    file_name_bob2,
    sizeof(file_name_bob2),
    L"%ws\\bob2.txt",
    buf)))
  {
    print("err, can not create file name");
    free(buf);
    return false;
  }
  free(buf); buf = NULL;

  if (true == is_file_existsW(file_name))
  {
    ::DeleteFileW(file_name);
  }

  // ÆÄÀÏ »ý¼º
  HANDLE file_handle = CreateFileW(
    file_name,
    GENERIC_WRITE,
    FILE_SHARE_READ,
    NULL,
    CREATE_NEW,
    FILE_ATTRIBUTE_NORMAL,
    NULL);
  if (file_handle == INVALID_HANDLE_VALUE)
  {
    print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
    return false;
  }

  // ÆÄÀÏ¿¡ µ¥ÀÌÅÍ ¾²±â
  DWORD bytes_written = 0;
  wchar_t string_buf[1024];
  if (!SUCCEEDED(StringCbPrintfW(
    string_buf,
    sizeof(string_buf),
    L"µ¿ÇØ¹°°ú ¹éµÎ»êÀÌ ¸¶¸£°í ´âµµ·Ï ÇÏ´À´ÔÀÌ º¸¿ìÇÏ»ç ¿ì¸®³ª¶ó¸¸¼¼")))
  {
    print("err, can not create data to write.");
    CloseHandle(file_handle);
    return false;
  }

  int bufLen;
  char *bufPtr = NULL;

  bufLen = WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, bufPtr, 0, NULL, NULL);
  bufPtr = (char*)malloc(bufLen + 1);
  WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, bufPtr, 1024, NULL, NULL);

  if (!WriteFile(file_handle, bufPtr, strlen(bufPtr), &bytes_written, NULL))
  {
    print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
    CloseHandle(file_handle);
    return false;
  }

  free(bufPtr);

  if (!SUCCEEDED(StringCbPrintfW(
    string_buf,
    sizeof(string_buf),
    L"All work and no play makes jack a dull boy.")))
  {
    print("err, can not create data to write.");
    CloseHandle(file_handle);
    return false;
  }

  bufLen = WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, bufPtr, 0, NULL, NULL);
  bufPtr = (char*)malloc(bufLen + 1);
  WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, bufPtr, 1024, NULL, NULL);

  if (!WriteFile(file_handle, bufPtr, strlen(bufPtr), &bytes_written, NULL))
  {
    print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
    CloseHandle(file_handle);
    return false;
  }

  // ÆÄÀÏ ´Ý±â
  CloseHandle(file_handle);

  CopyFileW(file_name, file_name_bob2, false);

  HANDLE file_handle2 = CreateFileW(
    file_name_bob2,
    GENERIC_READ,
    0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);
  if (file_handle == INVALID_HANDLE_VALUE)
  {
    print("err, CreateFile(path=%ws), gle=0x%08x", file_name_bob2, GetLastError());
    return false;
  }

// È­¸é Ãâ·Â! 

//  DeleteFile(file_name);
//  DeleteFile(file_name_bob2);

  CloseHandle(file_handle2);
  return true;

}


int _tmain(int argc, _TCHAR* argv[])
{
  create_bob_txt();
  _getch();
  return 0;
}