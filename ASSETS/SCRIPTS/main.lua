run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/entityDefs.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/assetDefs.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/utilities.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/ship.lua")

LoadAssets()

local entity = LoadEntity(ShipDefs["blue_ship"])
gShip = Ship:Create({id = entity})


main = {
	[1] = {
		update = function()
			gShip:UpdateShip()
		end
	},
	[2] = {
		render = function()
			
		end
	},
}