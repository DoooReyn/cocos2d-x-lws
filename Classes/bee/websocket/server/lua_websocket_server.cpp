#include "bee/websocket/server/bee_websocket_server.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaEngine.h"
#include "lua.h"

static int lua_websocket_server_make(lua_State* L)
{
	(WsServer*)tolua_tousertype(L, 1, 0);
	return 0;
}

static int lua_websocket_server_create(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif


	argc = lua_gettop(L) - 1;
	if (argc == 3)
	{
		const char* arg0, *arg1, *arg2;
		std::string arg0_tmp, arg1_tmp, arg2_tmp;
		ok &= luaval_to_std_string(L, 2, &arg0_tmp, "cc.WsServer:create");
		if (!ok) goto tolua_lerror;
		arg0 = arg0_tmp.c_str();

		ok &= luaval_to_std_string(L, 3, &arg1_tmp, "cc.WsServer:create");
		if (!ok) goto tolua_lerror;
		arg1 = arg1_tmp.c_str();

		ok &= luaval_to_std_string(L, 4, &arg2_tmp, "cc.WsServer:create");
		arg2 = arg2_tmp.c_str();
		if (!ok) goto tolua_lerror;

		WsServer *server = WsServer::create(arg0, arg1, arg2);
		object_to_luaval<WsServer>(L, "cc.WsServer", (WsServer*)server);
		return 1;
	}

	luaL_error(L, "create wrong number of arguments: %d, was expecting %d\n", argc, 3);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_create'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_setLuaListener(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
	if (!cobj)
	{
#if COCOS2D_DEBUG >= 1
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_setLuaListener'", nullptr);
#endif
		return 0;
	}

	argc = lua_gettop(L) - 1;
	if (argc == 1)
	{
		if (!lua_isfunction(L, 2)) {
			luaL_error(L, "invalid 'listener' in function 'lua_websocket_server_setLuaListener'.\n");
			return 0;
		}
		LUA_FUNCTION listener = (toluafix_ref_function(L, 2, 0));
		cobj->setLuaListener(listener);
		return 1;
	}

	luaL_error(L, "create wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
			tolua_error(L, "#ferror in function 'lua_websocket_server_setLuaListener'.\n", &tolua_err);
#endif
	return 0;
}

static int lua_websocket_server_broadcast(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
	if (!cobj)
	{
#if COCOS2D_DEBUG >= 1
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_broadcast'", nullptr);
#endif
		return 0;
	}

	argc = lua_gettop(L) - 1;
	if (argc == 1)
	{
		std::string arg0_tmp;
		ok &= luaval_to_std_string(L, 2, &arg0_tmp, "cc.WsServer:broadcast");
		if (!ok) goto tolua_lerror;

		cobj->broadcast(arg0_tmp);
		return 1;
	}

	luaL_error(L, "create wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_broadcast'.\n", &tolua_err);
#endif
	return 0;
}

static int lua_websocket_server_sendTo(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
	if (!cobj)
	{
#if COCOS2D_DEBUG >= 1
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_sendTo'", nullptr);
#endif
		return 0;
	}

	argc = lua_gettop(L) - 1;
	if (argc == 2)
	{
		std::string arg0_tmp;
		ok &= luaval_to_std_string(L, 2, &arg0_tmp, "cc.WsServer:sendTo");
		if (!ok) goto tolua_lerror;

		std::string arg1_tmp;
		ok &= luaval_to_std_string(L, 3, &arg1_tmp, "cc.WsServer:sendTo");
		if (!ok) goto tolua_lerror;

		cobj->sendTo(arg0_tmp, arg1_tmp);
		return 1;
	}

	luaL_error(L, "create wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_sendTo'.\n", &tolua_err);
#endif
	return 0;
}

static int lua_websocket_server_start(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
	if (!cobj)
	{
#if COCOS2D_DEBUG >= 1
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_start'", nullptr);
#endif
		return 0;
	}

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		cobj->start();
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:start", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_start'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_pause(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_pause'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		cobj->pause();
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:pause", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_pause'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_resume(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_resume'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		cobj->resume();
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:resume", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_resume'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_shutdown(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_shutdown'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		cobj->shutdown();
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:shutdown", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_shutdown'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_paused(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_paused'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		bool ret = cobj->paused();
		tolua_pushboolean(L, ret);
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:paused", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_paused'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_running(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_running'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		bool ret = cobj->running();
		tolua_pushboolean(L, ret);
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:running", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_running'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_dir(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_dir'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		std::string ret = cobj->dir();
		tolua_pushstring(L, ret.c_str());
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:dir", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_dir'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_ip(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_ip'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		std::string ret = cobj->ip();
		tolua_pushstring(L, ret.c_str());
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:ip", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_ip'.\n", &tolua_err);
#endif
				return 0;
}

static int lua_websocket_server_port(lua_State* L)
{
	if (NULL == L)
		return 0;

	int argc = 0;
	WsServer* cobj;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "cc.WsServer", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (WsServer*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(L, "invalid 'cobj' in function 'lua_websocket_server_port'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (argc == 0)
	{
		std::string ret = cobj->port();
		tolua_pushstring(L, ret.c_str());
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.WsServer:port", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_websocket_server_port'.\n", &tolua_err);
#endif
				return 0;
}

TOLUA_API int lua_module_register_websocket_server(lua_State* L)
{
	lua_getglobal(L, "_G");
	if (lua_istable(L, -1))
	{
		tolua_open(L);
		tolua_usertype(L, "cc.WsServer");
		tolua_module(L, "cc", 0);
		tolua_beginmodule(L, "cc");
		
		tolua_cclass(L, "WsServer", "cc.WsServer", "cc.Ref", lua_websocket_server_make);
		tolua_beginmodule(L, "WsServer");

		tolua_function(L, "create", lua_websocket_server_create);
		tolua_function(L, "setLuaListener", lua_websocket_server_setLuaListener);
		tolua_function(L, "broadcast", lua_websocket_server_broadcast);
		tolua_function(L, "sendTo", lua_websocket_server_sendTo);
		tolua_function(L, "start", lua_websocket_server_start);
		tolua_function(L, "pause", lua_websocket_server_pause);
		tolua_function(L, "resume", lua_websocket_server_resume);
		tolua_function(L, "shutdown", lua_websocket_server_shutdown);
		tolua_function(L, "paused", lua_websocket_server_paused);
		tolua_function(L, "running", lua_websocket_server_running);
		tolua_function(L, "dir", lua_websocket_server_dir);
		tolua_function(L, "ip", lua_websocket_server_ip);
		tolua_function(L, "port", lua_websocket_server_port);

		tolua_endmodule(L);

		tolua_endmodule(L);
	}
	lua_pop(L, 1);
	return 1;
}