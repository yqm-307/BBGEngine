package.path =      ";../?.lua;" .. package.path
package.path =      ";../../../share/script/?.lua;" .. package.path

require("preload.ComponentMgr")


ComponentMgr:RegistComponent(1, function (Args)
    print("on update")
end);

ComponentMgr:OnUpdateEvent(1);