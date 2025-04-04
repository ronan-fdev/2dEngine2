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
			DrawLine(Line(vec2(50, 50), vec2(200, 200), Color(255, 0, 0, 255 )))
 			DrawLine(Line(vec2(200, 50), vec2(50, 200), Color(0, 255, 0, 255 )))
 			DrawRect(Rect(vec2(300, 300), 100, 100, Color(0, 0, 255, 255)))
 			DrawRect(Rect(vec2(300, 300), 100, 100, Color(0, 0, 255, 255)))
 			DrawCircle(vec2(200, 200), 0.5, 200.0, Color(0, 0, 255, 255))
		end
	},
}