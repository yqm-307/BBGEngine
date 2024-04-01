local _g            = require("preload.Global")         --[[@as GTableDef]]
local _Tools        = require("preload.Tools")          --[[@as Tools]]

---@class ComponentMgr
---@field OnUpdateEvent     fun(self:ComponentMgr, nComponentId:ComponentId):boolean,string|nil # 由lua script comp调用，调用组件的lua事件
---@field RegistComponent   fun(self:ComponentMgr, nComponentId:ComponentId, fnCallback:CommonCallback):boolean,string|nil # 由lua script comp调用，注册组件c++事件回调
---@field UnRegistComponent fun(self:ComponentMgr, nComponentId:ComponentId) # 由lua script comp调用，反注册组件c++事件回调
---@field tbUpdateComponentEventMap table<ComponentId, CommonCallback>

---@class ComponentMgr
local ComponentMgr = _g.ComponentMgr

ComponentMgr.tbUpdateComponentEventMap = {}


---组件Update事件，由c++调用
---@param nComponentId integer
function ComponentMgr:OnUpdateEvent(nComponentId)
    local fnCallback = self.tbUpdateComponentEventMap[nComponentId]
    if not fnCallback then
        return false, ""
    end

    return _Tools.SafeCall(fnCallback)
end

function ComponentMgr:RegistComponent(nComponentId, fnCallback)
    local tbEvent = ComponentMgr.tbUpdateComponentEventMap[nComponentId]
    if tbEvent then
        return false, "regist repeat!"
    end

    ComponentMgr.tbUpdateComponentEventMap[nComponentId] = fnCallback
    return true
end

---反注册组件事件，由c++调用
---@param nComponentId ComponentId
function ComponentMgr:UnRegistComponent(nComponentId)
    self.tbUpdateComponentEventMap[nComponentId] = nil
end

return ComponentMgr