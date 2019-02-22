#include <errno.h>
#include <string.h>
#include <node_api.h>
#include <napi-macros.h>
#include <unistd.h>

#define NAPI_UTF8_GET_LENGTH(name, val) \
  NAPI_STATUS_THROWS(napi_get_value_string_utf8(env, val, NULL, 0, &name));

const char* err_name(int status) {
  switch (status) {
    case E2BIG: return "E2BIG";
    case ENOENT: return "ENOENT";
    case EINVAL: return "EINVAL";
    case EPERM: return "EPERM";
    default: return "UNKNOWN";
  }
}

#define MAX_PATH_LEN 1023
#define MAX_PERM_LEN 4

NAPI_METHOD(napi_openbsd_unveil) {
  NAPI_ARGV(2);

  napi_valuetype arg_type = napi_undefined;

  size_t path_len = 0;
  char path[MAX_PATH_LEN + 1];
  if (argc > 0) {
    NAPI_STATUS_THROWS(napi_typeof(env, argv[0], &arg_type));

    switch (arg_type) {
      case napi_null:
      case napi_undefined:
      case napi_string:
        break;
      default:
        napi_throw_error(env, "EINVAL", "path must be string or null");
        return NULL;
    }

    if (arg_type == napi_string) {
      NAPI_UTF8_GET_LENGTH(path_len, argv[0]);

      if (path_len > MAX_PATH_LEN) {
        napi_throw_error(env, "EINVAL", "path string too long");
        return NULL;
      }

      NAPI_STATUS_THROWS(napi_get_value_string_utf8(env, argv[0], path, MAX_PATH_LEN + 1, &path_len));
    }
  }

  size_t permissions_len = 0;
  char permissions[MAX_PERM_LEN + 1];
  if (argc > 1) {
    NAPI_STATUS_THROWS(napi_typeof(env, argv[1], &arg_type));

    switch (arg_type) {
      case napi_null:
      case napi_undefined:
      case napi_string:
        break;
      default:
        napi_throw_error(env, "EINVAL", "permissions must be string or null");
        return NULL;
    }

    if (arg_type == napi_string) {
      NAPI_UTF8_GET_LENGTH(permissions_len, argv[1]);

      if (permissions_len > MAX_PERM_LEN) {
        napi_throw_error(env, "EINVAL", "permissions string too long");
        return NULL;
      }

      NAPI_STATUS_THROWS(napi_get_value_string_utf8(env, argv[1], permissions, MAX_PERM_LEN + 1, &permissions_len));
    }
  }

  int err = unveil(path_len > 0 ? path : NULL, permissions_len > 0 ? permissions : NULL);
  if (err < 0) {
    napi_throw_error(env, err_name(errno), strerror(errno));
    return NULL;
  }

  return NULL;
}

NAPI_INIT() {
  NAPI_EXPORT_FUNCTION(napi_openbsd_unveil)
}
