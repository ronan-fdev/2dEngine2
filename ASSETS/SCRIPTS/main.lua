run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/entityDefs.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/assetDefs.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/utilities.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/ship.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/asteroid.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/collision_system.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/projectile.lua")
run_script("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SCRIPTS/ASTERIODS/game_data.lua")

math.randomseed(os.time())

LoadAssets()
LoadBackground()

local entity = LoadEntity(ShipDefs["blue_ship"])
gShip = Ship:Create({id = entity})
gCollisionSystem = CollisionSystem:Create()

main = {
	[1] = {
		update = function()
			gShip:UpdateShip()
			UpdateAsteroids()
			UpdateProjectiles()
			gCollisionSystem:Update()
			SpawnAsteroid()
			print("SCORE :"..gData:GetScore())
		end
	},
	[2] = {
		render = function()
			
		end
	},
}