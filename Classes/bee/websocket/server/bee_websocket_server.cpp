#include "bee_websocket_server.h"

#define CS_TAG "===============> WebSocketServer : "
#define WS_LOG(format, ...)	cocos2d::log(CS_TAG format, ##__VA_ARGS__)

static void __serve__(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message *hm = (struct mg_http_message *)ev_data;
		if (mg_http_match_uri(hm, "/websocket")) {
			// 升级到 websocket 协议
			mg_ws_upgrade(c, hm, NULL);
		}
		else {
			// 提供HttpServer的静态服务
			struct mg_mgr *mgr = (struct mg_mgr *)fn_data;
			WsServer *server = (WsServer *)mgr->userdata;
			if (server != NULL) {
				struct mg_http_serve_opts opts;
				std::string dir = server->dir();
				opts.root_dir = dir.c_str();
				mg_http_serve_dir(c, (mg_http_message *)ev_data, &opts);
			}
		}
	}
	else if (ev == MG_EV_WS_MSG) {
		// 收到WebSocket消息，缓存起来发给Lua层
		struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
		struct mg_mgr *mgr = (struct mg_mgr *)fn_data;
		WsServer *server = (WsServer *)mgr->userdata;
		if (server != NULL) {
			string msg;
			msg.assign(wm->data.ptr, wm->data.len);
			server->onReceive(c->id, msg);
		}
	}
}

WsServer* WsServer::create(std::string dir, std::string ip, std::string port)
{
	WsServer * ret = new (std::nothrow) WsServer(dir, ip, port);
	if (ret && ret->init(dir, ip, port))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

WsServer::~WsServer()
{
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
	CC_SAFE_DELETE(m_thread);
}

bool WsServer::init(std::string dir, std::string ip, std::string port)
{
	retain();

	m_thread = new std::thread(&WsServer::_runWithThread, this);
	m_thread->detach();

	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

	return true;
}

void WsServer::start()
{
	if (m_state == State::None) {
		m_state = State::Running;
	}
}

void WsServer::pause()
{
	if (m_state == State::Running) {
		m_state = State::Paused;
	}
}

void WsServer::resume() {
	if (m_state == State::Paused) {
		m_state = State::Running;
	}
}

void WsServer::shutdown() {
	if (m_state != State::Dead) {
		m_state = State::Dead;
	}
}

void WsServer::update(float dt)
{
	if (m_state != State::Running) return;
	if (m_listener <= -1) return;
	if (m_ws_msg.size() == 0) return;

	LuaStack *stack = LuaEngine::getInstance()->getLuaStack();
	if (!stack) return;

	LuaValueDict dict;
	WsMsg msg = m_ws_msg.front();
	dict["type"] = LuaValue::stringValue("ws");
	dict["data"] = LuaValue::stringValue(msg.msg);
	dict["from"] = LuaValue::stringValue(to_string(msg.id));
	m_ws_msg.pop_front();
	stack->pushLuaValueDict(dict);
	stack->executeFunctionByHandler(m_listener, 1);
	stack->clean();
}

void WsServer::onReceive(unsigned long client_id, std::string message) {
	WsMsg msg;
	msg.id = client_id;
	msg.msg = message;
	m_ws_msg.push_back(msg);
}

void WsServer::broadcast(string message) {
	if (m_state != State::Running) 
	{
		WS_LOG("广播无效，服务器未运行");
		return;
	}

	const char * msg = message.c_str();
	int len = message.size();
	for (struct mg_connection *c = m_mgr.conns; c != NULL; c = c->next) {
		if (c->is_websocket) 
			mg_ws_send(c, msg, len, WEBSOCKET_OP_TEXT);
	}
}

void WsServer::sendTo(string client_id, string message) {
	if (m_state != State::Running)
	{
		WS_LOG("发送失败，服务器未运行");
		return;
	}

	unsigned long id = strtoul(client_id.c_str(), NULL, 10);
	const char * msg = message.c_str();
	int len = message.size();
	for (struct mg_connection *c = m_mgr.conns; c != NULL; c = c->next) {
		if (c->is_websocket && id == c->id) {
			mg_ws_send(c, msg, len, WEBSOCKET_OP_TEXT);
			break;
		}
	}
}

void WsServer::_onRefresh()
{
	struct mg_connection *c;

	mg_mgr_init(&m_mgr);
	m_mgr.userdata = (void *)this;

	std::string _url = "ws://" + m_ip + ":" + m_port;
	const char* url = _url.c_str();
	if ((c = mg_http_listen(&m_mgr, url, __serve__, &m_mgr)) == NULL) {
		WS_LOG("Cannot listen on [%s %s].", m_dir.c_str(), url);
		return;
	}

	WS_LOG("Starting WebSocketServer on [%s %s]", m_dir.c_str(), url);
	m_state = State::Running;

	while (m_state != State::Dead) {
		if (m_state == State::Running)
			mg_mgr_poll(&m_mgr, 1000);
	}

	WS_LOG("Exiting WebSocketServer on [%s %s]", m_dir.c_str(), url);
	mg_mgr_free(&m_mgr);
	release();
}

void WsServer::_runWithThread(void *userdata) {
	if (userdata) {
		static_cast<WsServer*>(userdata)->_onRefresh();
	}
}
