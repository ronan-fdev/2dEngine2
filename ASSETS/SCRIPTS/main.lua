run_script("ASSETS/SCRIPTS/TESTPROJECT1/assetdefs.lua")
run_script("ASSETS/SCRIPTS/TESTPROJECT1/samplemap.lua")
run_script("ASSETS/SCRIPTS/utilities.lua")
run_script("ASSETS/SCRIPTS/rain_generator.lua")
run_script("ASSETS/SCRIPTS/events/event_manager.lua")
 run_script("ASSETS/SCRIPTS/events/collision_event.lua")
 run_script("ASSETS/SCRIPTS/systems/trigger_system.lua")
run_script("ASSETS/TILEDMAP/FireBoyWaterGirl/fireboywatergirltiledmap.lua")
--run_script("ASSETS/SCRIPTS/follow_cam.lua")

LoadAssets(AssetDefs)

gStateStack = StateStack()

TitleState = {}
TitleState.__index = TitleState

GameState = {}
GameState.__index = GameState 

function GameState:Create(stack)
	local this = 
	{
		m_Stack = stack 
	}
	
	local state = State("game state")
	state:set_variable_table(this)
	state:set_on_enter(
		function()
			print("Enter Game State")
		end
	)
	state:set_on_exit(
		function()
			print("Exit Game State")
		end
	)

	state:set_on_update(
		function(dt)
			print("Update GameState")
		end
	)

	state:set_on_render(
		function()
			print("Render Game State")
		end
	)

	state:set_handle_inputs(
		function()
			this:HandleInputs()
		end
	)

	setmetatable(this, self)
	return state
end

function GameState:HandleInputs()
	if Keyboard.just_released(KEY_BACKSPACE) then 
		self.m_Stack:pop()
		return 
	end
end

function TitleState:Create(stack)
	local this = 
	{
		m_Stack = stack,
		m_Title = Entity("title", ""),
		m_PressEnter = Entity("press_enter", "")
	}
	
	this.m_Title:add_component(Transform(vec2(100, 100), vec2(1, 1), 0))
	this.m_Title:add_component(TextComponent("pixel", "Title State"))

	this.m_PressEnter:add_component(Transform(vec2(100, 200), vec2(1, 1), 0))
	this.m_PressEnter:add_component(TextComponent("pixel", "PRESS ENTER!"))

	local state = State("title state")
	state:set_variable_table(this)
	state:set_on_enter(
		function()
			print("Enter Title State")
		end
	)
	state:set_on_exit(
		function()
			this:OnExit()
		end
	)

	state:set_on_update(
		function(dt)
			print("Update Title State")
		end
	)

	state:set_on_render(
		function()
			print("Render Title State")
		end
	)

	state:set_handle_inputs(
		function()
			this:HandleInputs()
		end
	)

	setmetatable(this, self)
	return state
end

function TitleState:OnExit()
	self.m_Title:kill()
	self.m_PressEnter:kill()
	-- TODO: 
end

function TitleState:HandleInputs()
	if (Keyboard.just_released(KEY_ENTER)) then 
		self.m_Stack:change_state(GameState:Create(self.m_Stack))
		return;
	end
end

local title = TitleState:Create(gStateStack)
gStateStack:change_state(title)

main = {
	[1] = {
		update = function()
			--[[
            rainGen:Update(0.016) 
            UpdatePlayer(gPlayer)
            --gFollowCam:update()

            --Checking some collision and trigger stuff.
			
            local uda, udb = ContactListener.get_user_data()
 			if uda and udb then 
 				gCollisionEvent:EmitEvent(uda, udb)
 			end 
			]]
            Debug()

			gStateStack:update(0.016)

		end
	},
	[2] = {
		render = function()
			gStateStack:render()
		end
	},
}