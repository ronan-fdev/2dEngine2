run_script("ASSETS/SCRIPTS/TESTPROJECT1/assetdefs.lua")
run_script("ASSETS/SCRIPTS/TESTPROJECT1/samplemap.lua")
run_script("ASSETS/SCRIPTS/utilities.lua")
run_script("ASSETS/SCRIPTS/rain_generator.lua")
run_script("ASSETS/SCRIPTS/events/event_manager.lua")
 run_script("ASSETS/SCRIPTS/events/collision_event.lua")
 run_script("ASSETS/SCRIPTS/systems/trigger_system.lua")
 run_script("ASSETS/SCRIPTS/GAME_STATES/game_state.lua")
 run_script("ASSETS/SCRIPTS/GAME_STATES/title_state.lua")
run_script("ASSETS/TILEDMAP/FireBoyWaterGirl/fireboywatergirltiledmap.lua")
--run_script("ASSETS/SCRIPTS/follow_cam.lua")

LoadAssets(AssetDefs)

gCollisionEvent = nil 
gTriggerSystem = nil 
gPlayer = nil 
rainGen = nil

gStateStack = StateStack()



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