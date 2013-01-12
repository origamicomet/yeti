#include <lwe/foundation/fsearch.h>
#include <lwe/foundation/platforms/windows.h>
#include <lwe/foundation/allocator.h>
#include <lwe/foundation/path.h>

static bool _lwe_fsearch(
  const lwe_native_str_t path,
  void* user_ptr,
  lwe_fsearch_callback_t on_found )
{
  const size_t path_len = wcslen(path);

  // Use Win32 to recursively iterate over files:
  WIN32_FIND_DATAW find_data;
  HANDLE handle = FindFirstFileW(path, &find_data);
  if (handle == INVALID_HANDLE_VALUE)
    return FALSE;

  do {
    if (find_data.cFileName[0] == '.')
      continue;

    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      const size_t file_path_len = wcslen(find_data.cFileName);

      lwe_native_char_t search_path[LWE_MAX_PATH];
      wcsncpy(&search_path[0], path, path_len);
      wcsncpy(&search_path[path_len - 1], find_data.cFileName, file_path_len);
      search_path[path_len + file_path_len - 1] = '/';
      search_path[path_len + file_path_len + 0] = '*';
      search_path[path_len + file_path_len + 1] = '\0';

      _lwe_fsearch(search_path, user_ptr, on_found);
      continue;
    }

    char file_path[LWE_MAX_PATH]; {
      const lwe_size_t num_bytes = lwe_from_native_str(find_data.cFileName, -1, NULL);
      const lwe_size_t offset = lwe_from_native_str(path, -1, &file_path[0]) - 2;

      lwe_from_native_str(
        find_data.cFileName, -1,
        &file_path[offset]
      );
    }

    on_found(user_ptr, file_path);
  } while (FindNextFileW(handle, &find_data));

  FindClose(handle);
  return TRUE;
}

bool lwe_fsearch(
  lwe_const_str_t path,
  void* user_ptr,
  lwe_fsearch_callback_t on_found )
{
  lwe_assert(path != NULL);
  lwe_assert(on_found != NULL);

  lwe_native_str_t native_path = NULL; {
    const lwe_size_t num_bytes = lwe_to_native_str(path, -1, NULL);
    native_path = (lwe_native_str_t)lwe_stack_alloc(num_bytes + 2 * sizeof(lwe_native_char_t));
    lwe_to_native_str(path, -1, native_path);

    lwe_native_str_t suffix =
      (lwe_native_str_t)((uint8_t*)native_path + num_bytes - sizeof(lwe_native_char_t));

    suffix[0] = '/'; suffix[1] = '*'; suffix[2] = '\0';
  }

  return _lwe_fsearch(native_path, user_ptr, on_found);
}

