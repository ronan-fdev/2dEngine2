run_script("ASSETS/SCRIPTS/TESTPROJECT1/assetdefs.lua")
run_script("ASSETS/SCRIPTS/TESTPROJECT1/samplemap.lua")
run_script("ASSETS/SCRIPTS/utilities.lua")

local tilemap = CreateTestMap()
 assert(tilemap)
 LoadAssets(AssetDefs)
 LoadMap(tilemap)

main = {
	[1] = {
		update = function()
			
		end
	},
	[2] = {
		render = function()
			
		end
	},
}