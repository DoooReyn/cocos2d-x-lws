#ifndef __lua_websocket_server_h__
#define __lua_websocket_server_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int lua_module_register_websocket_server(lua_State* tolua_S);

#endif // __lua_websocket_server_h__
