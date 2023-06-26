--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR ngcod
-- @DATE 2023/6/24
--

---@type MainUI_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self.btnCreate.OnPressed:Add(self, M.OnClickCreate)
    self.btnJoin.OnPressed:Add(self, M.OnClickJoin)
end

function M:OnClickCreate()
    local bUseLan = true
    local maxPlayer = 4
    local mapName = "CQB近战"

    local PlayerController = UE.UGameplayStatics.GetPlayerController(self, 0)
    local SessionProxy = UE.UNCreateSessionCallbackProxy.CreateSession(self, PlayerController, maxPlayer, bUseLan, mapName)
    SessionProxy.OnSuccess = {self, M.OnCreateSuccess}
    SessionProxy.OnFailure = {self, M.OnCreateFailure}
    SessionProxy:Activate()
end

function M:OnCreateSuccess()
    UE.UGameplayStatics.OpenLevel(self, '/Game/Maps/LobbyLevel', true, 'listen')
end

function M:OnCreateFailure()
    print('创建失败')
end

function M:OnClickJoin()
    self.SessionList:SetVisibility(UE.ESlateVisibility.Visible)
    self:SetMain(false)
    local PlayerController = UE.UGameplayStatics.GetPlayerController(self, 0)
    local SessionProxy = UE.UFindSessionsCallbackProxy.FindSessions(self, PlayerController, 100, true)
    SessionProxy.OnSuccess = {self, M.OnFindSessionSuccess}
    SessionProxy.OnFailure = {self, M.OnFindSessionFailure}
    SessionProxy:Activate()
end

function M:SetMain(bShow)
    if bShow then
        self.btnCreate:SetVisibility(UE.ESlateVisibility.Visible)
        self.btnJoin:SetVisibility(UE.ESlateVisibility.Visible)
    else
        self.btnCreate:SetVisibility(UE.ESlateVisibility.Hidden)
        self.btnJoin:SetVisibility(UE.ESlateVisibility.Hidden)
    end
end

function M:OnFindSessionSuccess(sessions)
    self.SessionList:BP_SetListItems(sessions)
end

function M:OnFindSessionFailure()
    print('查询失败')
    self.SetMain(true)
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
