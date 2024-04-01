local _g = require("preload.Global.lua") --[[@as GTableDef]]

---@class LuaScriptMgr
---@field tbEventMap table<integer, fun()>

--[[
    c++ engine component 调用 lua脚本的接口类
]]
---@class LuaScriptMgr
local LuaScriptMgr = _g.LuaScriptMgr or {}

LuaScriptMgr.tbEventMap = {}