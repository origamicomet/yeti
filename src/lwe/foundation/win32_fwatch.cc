// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#include <lwe/foundation/fwatch.h>
#include <lwe/foundation/platforms/windows.h>
#include <lwe/foundation/allocator.h>
#include <lwe/foundation/path.h>

struct lwe_fwatch_t {
  OVERLAPPED overlapped;
  HANDLE dir_handle;
  uint8_t buffer[65535];
  unsigned stopped : 1;
  unsigned reserved : 31;
  void* user_ptr;
  lwe_fwatch_callback_t on_modified;
};

static const DWORD _notify_filter =
  FILE_NOTIFY_CHANGE_CREATION |
  FILE_NOTIFY_CHANGE_SIZE |
  FILE_NOTIFY_CHANGE_FILE_NAME;

static void CALLBACK _directory_changes_callback(
  DWORD error_code,
  DWORD num_bytes_transfered,
  LPOVERLAPPED overlapped )
{
  lwe_fwatch_t* fw = (lwe_fwatch_t*)overlapped;

  if (num_bytes_transfered == 0)
    return;

  if (error_code == ERROR_SUCCESS) {
    lwe_size_t offset = 0;
    PFILE_NOTIFY_INFORMATION notify;

    do {
      notify = (PFILE_NOTIFY_INFORMATION)&fw->buffer[offset];
      offset += notify->NextEntryOffset;

      char path[LWE_MAX_PATH + 1] = { 0, }; {
        int num_bytes = WideCharToMultiByte(
          CP_UTF8, 0,
          notify->FileName, notify->FileNameLength / sizeof(wchar_t),
          &path[0], MAX_PATH, 0, 0
        );

        path[num_bytes] = '\0';
      }

      lwe_fwatch_event_t event;

      switch( notify->Action ) {
        case FILE_ACTION_RENAMED_NEW_NAME:
        case FILE_ACTION_ADDED:
          event = LWE_FWATCH_EVENT_CREATED;
        break;

        case FILE_ACTION_RENAMED_OLD_NAME:
        case FILE_ACTION_REMOVED:
          event = LWE_FWATCH_EVENT_DESTROYED;
        break;

        case FILE_ACTION_MODIFIED:
          event = LWE_FWATCH_EVENT_MODIFIED;
        break;
      }

      fw->on_modified(fw->user_ptr, event, (lwe_const_str_t)&path[0]);
    } while (notify->NextEntryOffset != 0 && !fw->stopped);
  }

  if (!fw->stopped) {
    ReadDirectoryChangesW(
      fw->dir_handle, &fw->buffer[0], sizeof(fw->buffer),
      TRUE, _notify_filter, NULL, &fw->overlapped,
      &_directory_changes_callback
    );
  }
}

lwe_fwatch_t* lwe_fwatch_start(
  lwe_const_str_t path,
  void* user_ptr,
  lwe_fwatch_callback_t on_modified )
{
  lwe_assert(path != NULL);
  lwe_assert(on_modified != NULL);
  
  lwe_native_str_t native_path = NULL; {
    const lwe_size_t num_bytes = lwe_to_native_str(path, -1, NULL);
    native_path = (lwe_native_str_t)lwe_stack_alloc(num_bytes);
    lwe_to_native_str(path, -1, native_path);
  }

  HANDLE dir_handle = CreateFileW(
    native_path,
    FILE_LIST_DIRECTORY,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL, OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL
  );

  if (dir_handle == INVALID_HANDLE_VALUE)
    return NULL;

  HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);

  if (!event) {
    CloseHandle(dir_handle);
    return NULL;
  }

  lwe_fwatch_t* fw = (lwe_fwatch_t*)lwe_alloc(sizeof(lwe_fwatch_t));
  ZeroMemory((void*)fw, sizeof(lwe_fwatch_t));

  fw->user_ptr = user_ptr;
  fw->on_modified = on_modified;
  fw->dir_handle = dir_handle;
  fw->overlapped.hEvent = event;
  fw->stopped = FALSE;

  const BOOL success = ReadDirectoryChangesW(
    fw->dir_handle, &fw->buffer[0], sizeof(fw->buffer),
    TRUE, _notify_filter, NULL, &fw->overlapped,
    &_directory_changes_callback
  );

  if (!success) {
    lwe_free((void*)fw);
    CloseHandle(event);
    CloseHandle(dir_handle);
    return NULL;
  }

  return fw;
}

void lwe_fwatch_poll(
  lwe_fwatch_t* fw )
{
  lwe_assert(fw != NULL);
  MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
}

void lwe_fwatch_stop(
  lwe_fwatch_t* fw )
{
  lwe_assert(fw != NULL);

  CancelIo(fw->dir_handle);
  ReadDirectoryChangesW(
    fw->dir_handle, &fw->buffer[0], sizeof(fw->buffer),
    FALSE, _notify_filter, NULL, &fw->overlapped, NULL
  );

  if (!HasOverlappedIoCompleted(&fw->overlapped))
    WaitForSingleObjectEx(fw->overlapped.hEvent, INFINITE, TRUE);

  CloseHandle(fw->overlapped.hEvent);
  CloseHandle(fw->dir_handle);
  lwe_free((void*)fw);
}