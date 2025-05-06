run_script("ASSETS/SCRIPTS/TESTPROJECT1/script_list.lua")

load_script_table(ScriptList)

LoadAssets(AssetDefs)

gCollisionEvent = nil 
gTriggerSystem = nil 
gPlayer = nil 
rainGen = nil

gStateStack = StateStack()



local title = TitleState:Create(gStateStack)
gStateStack:change_state(title)

LUA_TRACE("Hello Lua Logging into LUA_TRACE!")  
LUA_INFO("Hello Lua Logging into LUA_INFO!")   
LUA_WARN("Hello Lua Logging into LUA_WARN!")   
LUA_ERROR("Hello Lua Logging! into LUA_ERROR")  
LUA_FATAL("Hello Lua Logging! into LUA_FATAL")  

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