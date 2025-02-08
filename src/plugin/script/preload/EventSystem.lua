local _g            = require("preload.Global") --[[@as GTableDef]]
local _Tools        = require("preload.Tools")

---@alias EventId integer

---@class EventSystem
local EventSystem = _g.EventSystem

EventSystem.EVENT = {
    EM_COMPONENT_UPDATE = 1, -- lua script 组件 update
}

---@type table<EventId, table<Object, {fnCallback:fun(...), Args:...}>>
EventSystem.tbEventFuncMap = {}

function EventSystem:NotifyEvent(nEventId, ...)
    if not self.EVENT[nEventId] then
        return false
    end

    local tbEventParams = self.tbEventFuncMap[nEventId]
    if not tbEventParams then
        _Tools.LogErr("event not exist")
        return false
    end

    for _, tbInfo in pairs(tbEventParams) do
        _Tools.SafeCall(tbInfo.fnCallback, table.unpack(tbInfo.Args))
    end

    return true
end


---监听事件
---@param nEventId  EventId # 监听的事件id
---@param pListener Object  # 监听者对象
---@param fnOnEvent fun(pListener:Object, Args:...) # 事件回调闭包
---@param ...       any     # 事件回调闭包参数
---@return boolean # 是否注册成功
function EventSystem:ListenEvent(nEventId, pListener, fnOnEvent, ...)
    if not self.EVENT[nEventId] then
        _Tools.LogErr("event id not exist!")
        return false
    end

    --- 该事件监听列表是否存在
    local tbEventListenerList = self.tbEventFuncMap[nEventId]
    if not tbEventListenerList then
        tbEventListenerList = {}
        self.tbEventFuncMap[nEventId] = tbEventListenerList
    end

    -- 监听者是否已经存在
    if tbEventListenerList[pListener] then
        return false
    end

    tbEventListenerList[pListener] = {fnCallback = fnOnEvent, Args = ...}

    return true
end


function EventSystem:UnListenEvent()
    
end