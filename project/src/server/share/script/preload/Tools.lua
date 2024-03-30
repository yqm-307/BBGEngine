---@class Tools
local Tools = {}

---错误输出日志
---@param szLogMsg string
function Tools.LogErr(szLogMsg)
    io.write(io.stderr, szLogMsg)
end

---安全调用
---@param fnCallback CommonCallback
---@param ... any
---@return boolean # 函数调用是否执行成功
function Tools.SafeCall(fnCallback, ...)
    local fnMessageHandler = function (szMsg)
        Tools.LogErr(debug.traceback(szMsg, 2))
        return szMsg
    end

    if type(fnCallback) ~= "function" then
        return false
    end

    local bSucc = xpcall(fnCallback, fnMessageHandler, table.unpack(...))
    return bSucc
end

return Tools