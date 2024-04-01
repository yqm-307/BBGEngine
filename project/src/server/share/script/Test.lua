local _ComponentMgr = require("preload.ComponentMgr")
local _Tools        = require("preload.Tools")
local _TargetScript = require("%s")


if not _TargetScript.OnUpdate then
    _Tools.LogErr("请实现 OnUpdate 函数")
end

_ComponentMgr:RegistComponent("%d", _TargetScript.OnUpdate)