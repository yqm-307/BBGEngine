package.path =      ";../?.lua;" .. package.path
package.path =      ";../../../share/script/?.lua;" .. package.path

require("preload.ComponentMgr")


local function OnUpdate()
    print("component do onupdate function!")
end

return {
    OnUpdate = OnUpdate
}