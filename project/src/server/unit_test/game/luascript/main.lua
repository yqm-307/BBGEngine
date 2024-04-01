local _EventSystem = require("share.script.preload.EventSystem") --[[@as EventSystem]]

function Main(nValue)
    _EventSystem:ListenEvent()
    return nValue
end