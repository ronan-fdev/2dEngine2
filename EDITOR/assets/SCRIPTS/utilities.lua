-------------------------------------------------------------------
 -- Helper Functions
 -------------------------------------------------------------------
 --[[
 	Takes in an entity definition and builds a new entity by
 	adding the given components and values based on the provided 
 	definition.
 	Returns the newly created entity's ID
 --]]
function LoadEntity(def)
	assert(def, "Def does not exist")

	local tag = ""
	if def.tag then
		tag = def.tag 
	end

	local group = ""
	if def.group then 
		group = def.group 
	end 

	local newEntity = Entity(tag, group)

	if def.components.transform then
		newEntity:add_component(
			Transform(
				vec2(
					def.components.transform.position.x,
					def.components.transform.position.y
				),
				vec2(
					def.components.transform.scale.x,
					def.components.transform.scale.y
				),
				def.components.transform.rotation
			)
		)
	end

	if def.components.sprite then
		local sprite = newEntity:add_component(
			Sprite(
				def.components.sprite.asset_name,
				def.components.sprite.width,
				def.components.sprite.height,
				def.components.sprite.start_x,
				def.components.sprite.start_y,
				def.components.sprite.layer
			)
		)
		sprite:generate_uvs()
		sprite.bHidden = def.components.sprite.bHidden or false
	end

	if def.components.circle_collider then
		newEntity:add_component(
			CircleCollider(
				def.components.circle_collider.radius,
               def.components.circle_collider.offset
			)
		)
	end

	if def.components.animation then 
		newEntity:add_component(
			Animation(
				def.components.animation.num_frames,
				def.components.animation.frame_rate,
				def.components.animation.frame_offset,
				def.components.animation.bVertical,
				def.components.animation.bLooped
			)
		)
	end
   if def.components.physics_attributes then 
		local physAttr = def.components.physics_attributes 
		local newPhysicsAttr = PhysicsAttributes()
		
		if physAttr.bInteractionType == BodyInteractionType.TRIGGER then
			newPhysicsAttr.bInteractionType = BodyInteractionType.TRIGGER
		elseif physAttr.bInteractionType == BodyInteractionType.COLLIDER then
			newPhysicsAttr.bInteractionType = BodyInteractionType.COLLIDER
		elseif physAttr.bInteractionType == BodyInteractionType.SENSOR then
			LUA_INFO("sensor initialization is being done.")
			newPhysicsAttr.bInteractionType = BodyInteractionType.SENSOR
		else
			newPhysicsAttr.bInteractionType = BodyInteractionType.NORMAL
		end

		newPhysicsAttr.eType = physAttr.type 		
		newPhysicsAttr.density = physAttr.density
		newPhysicsAttr.friction = physAttr.friction
		newPhysicsAttr.restitution = physAttr.restitution
		newPhysicsAttr.position = physAttr.position
		newPhysicsAttr.radius = physAttr.radius
		newPhysicsAttr.bCircle = physAttr.bCircle or false
		newPhysicsAttr.bFixedRotation = physAttr.bFixedRotation or false
        newPhysicsAttr.gravityScale = physAttr.gravityScale or -1
        newPhysicsAttr.scale = physAttr.scale

		newPhysicsAttr.objectData = ObjectData(
			physAttr.object_data.tag,
			physAttr.object_data.group,
			physAttr.object_data.bCollider,
			physAttr.object_data.bTrigger,
			physAttr.object_data.bSensor,
			newEntity:id()
		)
		
		newEntity:add_component(PhysicsComp(newPhysicsAttr))
	end

	return newEntity:id()
end

-- Returns a random vec2 based on the min/max speed arguments
function GetRandomVelocity(min_speed, max_speed)
	return vec2(
		math.random(min_speed, max_speed),
		math.random(min_speed, max_speed)
	)
end
 
 --[[
 	Gets the value of the specific digit for 
 	the number that is sent in. For instance,
 	if the number is 9596 and we want digit three,
 	it would return 5.
 --]]
 function GetDigit(num, digit)
 	local n = 10 ^ digit 
 	local n1 = 10 ^ (digit - 1)
 	return math.floor((num % n) / n1)
 end
 
 
--[[
	Tileset Lua class used for parsing necessary tileset info
	when loading maps created using the Tiled Map Editor.
--]]
Tileset = {}
Tileset.__index = Tileset
function Tileset:Create(params)
	local this = 
	{
		name = params.name, 
		columns = params.columns,
		width = params.width,
		height = params.height,
		tilewidth = params.tilewidth,
		tileheight = params.tileheight,
		firstgid = params.firstgid
	}

	this.rows = params.height / params.tileheight
	this.lastgid = math.floor(((this.rows * this.columns) + this.firstgid) - 1)

	setmetatable(this, self)
	return this
end

-- Checks to see if the given ID exists as a tile in the tileset.
-- returns true if the id is between the first and last gid
function Tileset:TileIdExists(id)
	return id >= self.firstgid and id <= self.lastgid
end

-- Gets the needed start_x and start_y values for the sprite component
-- When generating the uvs, the start x and start y are used to tell the 
-- engine where on the tilesheet the tile sprite is.
function Tileset:GetTileStartXY(id)
	assert(self:TileIdExists(id), "Tile ID[" ..id .."] does not exist in tileset[" ..self.name .."]")
	local actualTileID = id - self.firstgid
	local start_y = math.floor(actualTileID / self.columns)
	local start_x = math.floor(actualTileID % self.columns)
	return start_x, start_y
end

-- Loop through a tilesets table and check to see if that particular ID 
-- exists in a tileset. 
-- Returns the correct tileset if it exists, nil otherwise.
function GetTileset(tilesets, id)
	assert(tilesets)
	for k, v in pairs(tilesets) do 
		if v:TileIdExists(id) then 
			return v
		end 
	end 
	return nil
end

-- Takes in the Tilemap table returned from tilemaps created using Tiled Map Editor.
-- Creates a new tileset object for each tileset and then loads new tile entities based 
-- on their layer, tile id, and position.
function LoadMap(mapDef)
	assert(mapDef)
	local tilesets = {}
	for k, v in pairs(mapDef.tilesets) do 
		local newTileset = Tileset:Create(
			{
				name = v.name,
				columns = v.columns,
				width = v.imagewidth,
				height = v.imageheight,
				tilewidth = v.tilewidth,
				tileheight = v.tileheight,
				firstgid = v.firstgid
			}
		)

		table.insert(tilesets, newTileset)
	end

	-- Loop through the layers and create the tiles 
	for k, v in ipairs(mapDef.layers) do 
		local rows = v.height - 1 
		local cols = v.width 
		local layer = k - 1 

		for row = 0, rows do
			for col = 1, cols do
				local id = v.data[row * cols + col]

				if id == 0 then 
					goto continue 
				end 

                
				
				local tileset = GetTileset(tilesets, id)
				assert(tileset, "Tileset does not exist with id: " ..id)
				local start_x, start_y = tileset:GetTileStartXY(id)

                

				-- Scale is used for testing, will change this once zoom in camera is working.
				local scale = 1
				local position = vec2(((col - 1) * tileset.tilewidth * scale), (row * tileset.tileheight * scale))
               
                --print(id," ",row," ",col, " ", start_x, " ", start_y, " ",position.x," ",position.y)

				local tile = Entity("", "tiles")
				local transform = tile:add_component(Transform(position, vec2(scale, scale), 0))

				local bIsCollider = false

				-- Currently the box collider has a sprite, this will be changed once we can draw
				-- Simple Primitives in the engine.
				if tileset.name == "collider" or tileset.name == "trigger" or tileset.name == "sensor" then 
					tile:add_component(
						BoxCollider(
							tileset.tilewidth,
							tileset.tileheight,
							vec2(0, 0)
						)
					)

                    -- If Box2d is enable, create a physics component
					if IsPhysicsEnabled() then 
						local physicsAttribs = PhysicsAttributes()

						-- Adjust properties to desired values, properties that are unchanged use default
						physicsAttribs.eType = BodyType.Static
						physicsAttribs.density = 1000.0			
						physicsAttribs.friction = 0.0			
						physicsAttribs.restitution = 0.0		
						physicsAttribs.gravityScale = 0.0
						physicsAttribs.position = transform.position
						physicsAttribs.bFixedRotation = true
						physicsAttribs.boxSize = vec2(tileset.tilewidth, tileset.tileheight)
						physicsAttribs.bBoxShape = true

						if tileset.name == "trigger" then 
							--physicsAttribs.bIsSensor = true 
                           --physicsAttribs.bIsContactEventsEnabled = true
						   physicsAttribs.bInteractionType = BodyInteractionType.TRIGGER
                           physicsAttribs.objectData = ObjectData("", "hole_triggers", false, true, false, tile:id())
                        elseif tileset.name == "collider" then
                            --physicsAttribs.bIsContactEventsEnabled = true
                            physicsAttribs.objectData = ObjectData("", "colliders", true, false, false, tile:id())
							physicsAttribs.bInteractionType = BodyInteractionType.COLLIDER
						elseif tileset.name == "sensor" then
							physicsAttribs.objectData = ObjectData("", "sensors", false, false, true, tile:id())
							physicsAttribs.bInteractionType = BodyInteractionType.SENSOR
						end
                        tile:add_component(PhysicsComp(physicsAttribs))
					end
                   bIsCollider = true
				end

               -- If not a collider, create the tile sprite
				if not bIsCollider then 
					local sprite = tile:add_component(
						Sprite(
							tileset.name,
							tileset.tilewidth,
							tileset.tileheight,
							start_x, start_y,
							layer
						)
					)

					sprite:generate_uvs()
				end

				::continue::
			end
		end
	end
end

--[[
	Loads all the assets into the AssetManager that are defined in the 
	given assets table. 
	Expects a table in the form of 
		Assets = {
			textures { ... },
			music { ... },
			sound_fx { ... },
			-- ...Other asset types as needed
		}
--]]
function LoadAssets(assets)
	for k, v in pairs(assets.textures) do
		if not AssetManager.add_texture(v.name, v.path, v.pixel_art) then
			LUA_ERROR("Failed to load texture ",v.name," at path ",v.path)
			print()
		else
			LUA_INFO("Loaded Texture :",v.name)
		end
	end

	for k,v in pairs(assets.tileset) do
		if not AssetManager.add_tileset_texture(v.name, v.path) then
			LUA_ERROR("Failed to load texture ",v.name," at path ",v.path)
			print()
		else
			LUA_INFO("Loaded Texture :",v.name)
		end
	end

	for k, v in pairs(assets.music) do 
		if not AssetManager.add_music(v.name, v.path) then 
			LUA_ERROR("Failed to load music ",v.name," at path ",v.path)
		else
			LUA_INFO("Loaded Music :",v.name)
		end
	end

	for k, v in pairs(assets.sound_fx) do
		if not AssetManager.add_soundfx(v.name, v.path) then
			LUA_ERROR("Failed to load sound_fx ",v.name," at path ",v.path)
		else
			LUA_INFO("Loaded SoundFX :",v.name)
		end
	end

   for k, v in pairs(assets.font) do
      if not AssetManager.add_font(v.name, v.path, v.font_size) then
			LUA_ERROR("Failed to load font ",v.name," at path ",v.path)
      else
	        LUA_INFO("Loaded Font :",v.name)
      end 
   end
	-- TODO: Add other loading of assets as needed
end

-----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------

ActiveCharacters = {}

function AddActiveCharacter(entity_id, character)
	ActiveCharacters[entity_id] = character
end

function GetActiveCharacter(entity_id)
	assert(ActiveCharacters[entity_id], string.format("Character with ID [%d] does not exist.", entity_id))
	return ActiveCharacters[entity_id]
end

-- TODO: fix this to actually destroy the underlying entity
function ClearCharacters()
	for k, v in pairs(ActiveCharacters) do 
		ActiveCharacters[k] = nil 
	end 
end

function UpdateActiveCharacters(dt)
	for _, v in pairs(ActiveCharacters) do 
		v.m_Controller:update(dt)
	end 
end

----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

function Debug()
	if Keyboard.just_pressed(KEY_C) then 
		if IsRenderCollidersEnabled() then 
			DisableRenderColliders()
		else 
			EnableRenderColliders()
		end 
	end
end