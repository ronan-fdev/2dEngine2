ANIMATION_STATE = 
{
    IDLE = 0,
    RUN_LEFT = 1,
    RUN_RIGHT = 2,
    JUMP = 4
}

GameState = {}
GameState.__index = GameState

function GameState:Create(stack)
	local this = 
	{
		m_Stack = stack,
        playerAnimationState = ANIMATION_STATE.IDLE
	}

	local state = State("game state")
 	state:set_variable_table(this)
 	state:set_on_enter( function() this:OnEnter() end )
 	state:set_on_exit( function() this:OnExit() end )
 	state:set_on_update( function(dt) this:OnUpdate(dt) end )
 	state:set_on_render( function() this:OnRender() end )
 	state:set_handle_inputs( function() this:HandleInputs() end )
 
 	setmetatable(this, self)
 
 	this:Initialize()
 
 	return state
end

function GameState:Initialize()
    -- Create events
 	if not gCollisionEvent then 
 		gCollisionEvent = CollisionEvent:Create()
 	end 
 
 	if not gTriggerSystem then 
 		gTriggerSystem = TriggerSystem:Create()
 	end 
 
 	gCollisionEvent:SubscribeToEvent(gTriggerSystem)

     -- Create the player
    if not gPlayer then
       local character = Character:Create({ name = "main_player" })
 		gPlayer = Entity(character.m_EntityID)
 		AddActiveCharacter(gPlayer:id(), character)
    end

    if not rainGen then
       rainGen = RainGenerator:Create()
    end
end

function GameState:OnEnter()
    LoadMap(createFBWGWorld())
end

function GameState:OnExit()
end

function GameState:OnUpdate(dt)
    UpdateActiveCharacters(dt)
    self:UpdateContacts()
    rainGen:Update(dt) 
end

function GameState:OnRender()
end

function GameState:HandleInputs()
	if Keyboard.just_released(KEY_BACKSPACE) then 
		self.m_Stack:pop()
		return 
	end
end

function GameState:UpdateContacts()
	local uda, udb = ContactListener.get_user_data()
	if uda and udb then 
		gCollisionEvent:EmitEvent(uda, udb)
	end 
end
