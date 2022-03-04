#ifndef __bee_websocket_server_h__
#define __bee_websocket_server_h__

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "bee/thirds/mongoose.h"
#include "CCLuaEngine.h"

#ifdef _WINDOWS_
#include <Windows.h>
#else
#include <pthread.h>
#endif

using namespace std;
using namespace cocos2d;

class WsServer : public Ref {
public:
	struct WsMsg {
		unsigned long id;
		string msg;

		WsMsg() {
			id = 0;
			msg = "";
		}
	};
	enum State
	{
		None = 0,
		Running,
		Paused,
		Dead
	};
	static WsServer* create(string dir, string ip, string port);
	void start();
	void pause();
	void resume();
	void shutdown();
	bool paused() { return m_state == State::Paused; }
	bool running() { return m_state == State::Running; }
	void setLuaListener(LUA_FUNCTION listener) {
		m_listener = listener;
	}
	string dir() { return m_dir; }
	string ip() { return m_ip; }
	string port() { return m_port; }
	void sendTo(string client_id, string message);
	void broadcast(string message);
	void onReceive(unsigned long client_id, string message);
	virtual void update(float dt);

private:
	virtual ~WsServer();
	virtual bool init(string dir, string ip, string port);
	WsServer(string dir, string ip, string port)
		:m_dir(".")
		, m_ip("0.0.0.0")
		, m_port("16999")
		, m_state(State::None)
		, m_listener(-1)
	{
		if (dir.size() > 0) {
			m_dir = dir;
		}
		if (ip.size() > 0) {
			m_ip = ip;
		}
		if (port.size() > 0) {
			m_port = port;
		}
		m_ws_msg.clear();
	}
	void _onRefresh();
	static void _runWithThread(void *userdata);
private:
	string m_ip;
	string m_port;
	string m_dir;
	thread* m_thread;
	struct mg_mgr m_mgr;
	State m_state;
	LUA_FUNCTION m_listener;
protected:
	list<WsMsg> m_ws_msg;
};

#endif //__bee_websocket_server_h__