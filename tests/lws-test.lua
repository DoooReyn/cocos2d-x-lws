function startWebSocketServer()
    -- WebSocketServer指定的监听目录
    local server_dir = cc.FileUtils:getInstance():getWritablePath()

    -- WebSocketServer指定的IP，不填默认本机
    local server_ip = ''

    -- WebSocketServer指定的端口，默认16999
    local server_port = '8000'

    local server = cc.WsServer:create(server_dir, server_ip, server_port)
    if tolua.isnull(server) then
        print('cc.WsServer 创建失败')
        return
    end
    print('cc.WsServer 已创建')
    server:setLuaListener(
        function(dict)
            dump(dict, '[WebSocketServer] onReceive: ')
            if dict and dict.type == 'ws' then
                local msg = string.format('[直达] 我是服务端，我已收到你(%s)的消息(%s)', dict.from, dict.data)
                server:sendTo(dict.from, msg)
            end
            server:broadcast('[广播] 刷存在感: ' .. os.time())
        end
    )

    -- 输出WebSocketServer基础信息
    print('== cc.WsServer ==')
    print('cc.WsServer.dir ', server:dir())
    print('cc.WsServer.ip  ', server:ip())
    print('cc.WsServer.port', server:port())
    print('-------------------------')

    -- 启动WebSocketServer，此时网页可以访问了
    server:start()

    -- 查看WebSocketServer运行状态
    print('== cc.WsServer运行状态 ==')
    print('cc.WsServer.running', server:running())
    print('cc.WsServer.paused', server:paused())
    print('-------------------------')
end
