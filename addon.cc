#include <assert.h>
#include <node_api.h>
#include "src/dglib.h"


napi_value RunCallback(napi_env env, const napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  napi_value cb = args[0];

  napi_value argv[1];
  status = napi_create_string_utf8(env, "hello world", NAPI_AUTO_LENGTH, argv);
  assert(status == napi_ok);

  napi_value global;
  status = napi_get_global(env, &global);
  assert(status == napi_ok);

  napi_value result;
  status = napi_call_function(env, global, cb, 1, argv, &result);
  assert(status == napi_ok);

  return nullptr;
}
napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value world;
  status = napi_create_string_utf8(env, "world", 5, &world);
  assert(status == napi_ok);
  return world;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_value new_exports;
  napi_status status = napi_create_function(
      env, "", NAPI_AUTO_LENGTH, RunCallback, nullptr, &new_exports);
  assert(status == napi_ok);

  napi_property_descriptor desc = DECLARE_NAPI_METHOD("hello", Method);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);

  return new_exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)