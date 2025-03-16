run_script("ASSETS/SCRIPTS/ASTERIODS/utilities.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/entityDefs.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/assetDefs.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/ship.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/asteroid.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/collision_system.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/projectile.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/game_data.lua")
run_script("ASSETS/SCRIPTS/ASTERIODS/hud.lua")

math.randomseed(os.time())

LoadAssets()
LoadBackground()

SoundSystem:play_music_track("event:/BACKGROUND/background")

local entity = LoadEntity(ShipDefs["blue_ship"])
gShip = Ship:Create({id = entity})
local sound = Entity(entity):add_component(
	SoundListener(
		true
	)
)

gCollisionSystem = CollisionSystem:Create()
gHud = Hud:Create()

Music.play("space")

main = {
	[1] = {
		update = function()
			gShip:UpdateShip()
			UpdateAsteroids()
			UpdateProjectiles()
			gCollisionSystem:Update()
			gHud:Update()
			if not gData:IsGameOver() then
				SpawnAsteroid()
			else 
				if Keyboard.just_pressed(KEY_ENTER) then 
					gData:Reset()
					gHud:Reset()
					gShip:Reset()
					ResetAsteroids()
					ResetProjectiles()
				end
			end
		end
	},
	[2] = {
		render = function()
			
		end
	},
}