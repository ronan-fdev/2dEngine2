AssetDefs = 
{
	textures = 
	{
		{ name = "ship", path = "C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/TEXTURES/ship.png"},
		{ name = "ast_1", path = "C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/TEXTURES/asteroid_1.png"},
		{ name = "ast_2", path = "C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/TEXTURES/asteroid_2.png"},
	}
}
function LoadAssets()
	for k, v in pairs(AssetDefs.textures) do
		if not AssetManager.add_texture(v.name, v.path) then
			print("Failed to load texture [" ..v.name .."] at path [" ..v.path .."]")
		else
			print("Loaded Texture [" ..v.name .."]")
		end
	end
	-- TODO: Load other asset types
end