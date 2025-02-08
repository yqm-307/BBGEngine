local function OnUpdate()
    G_UPDATE_COUNT = G_UPDATE_COUNT + 1
end

return {
    OnUpdate = OnUpdate
}