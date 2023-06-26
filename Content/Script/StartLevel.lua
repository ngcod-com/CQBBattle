--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR ngcod
-- @DATE 2023/6/24
--

---@type StartLevel_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    print("StartLevel_C Initialize")
end

-- function M:UserConstructionScript()
-- end

function M:ReceiveBeginPlay()
    local UCLass = UE.UClass.Load('/Game/UI/MainUI.MainUI')
    if UCLass == nil then
        return
    end
    local child = UE.UWidgetBlueprintLibrary.Create(self, UCLass, nil)
    child:AddtoViewport(0)
    local PlayerController = UE.UGameplayStatics.GetPlayerController(self, 0)
    UE.UWidgetBlueprintLibrary.SetInputMode_GameAndUIEx(PlayerController, nil, 0, true)
    local PlayerController = UE.UGameplayStatics.GetPlayerController(self, 0)
    PlayerController.bShowMouseCursor = true
end

-- function M:ReceiveEndPlay()
-- end

-- function M:ReceiveTick(DeltaSeconds)
-- end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

return M
