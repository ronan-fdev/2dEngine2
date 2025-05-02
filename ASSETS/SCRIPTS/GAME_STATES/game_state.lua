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
       gPlayer = Entity("player", "")
       local playerTransform = gPlayer:add_component(Transform(vec2(528,224), vec2(0.25,0.25),0))
       local sprite = gPlayer:add_component(Sprite("fireboy", 128, 128, 0, 0, 2))
       sprite:generate_uvs()
       local animation = gPlayer:add_component(Animation(2,6,0,false,true))

       local circleCollider = gPlayer:add_component(CircleCollider(64, vec2(0, 0)))

       local playerPhysAttr = PhysicsAttributes()
       playerPhysAttr.eType = BodyType.Dynamic
       playerPhysAttr.density = 75.0
       playerPhysAttr.friction = 1.0
       playerPhysAttr.restitution = 0.0
       playerPhysAttr.position = playerTransform.position 
       playerPhysAttr.radius = circleCollider.radius 
       playerPhysAttr.gravityScale = -2
       playerPhysAttr.scale = playerTransform.scale
       playerPhysAttr.bCircle = true 
       playerPhysAttr.bFixedRotation = true 
       playerPhysAttr.objectData = (ObjectData("player", "", true, false, gPlayer:id()))
       --playerPhysAttr.bIsContactEventsEnabled = true

       -- Add Physics component to the player 
       gPlayer:add_component(PhysicsComp(playerPhysAttr))
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
    self:UpdatePlayer(gPlayer)
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

function GameState:UpdatePlayer(player)
    local physics = player:get_component(PhysicsComp)
    local transform = player:get_component(Transform)
    local sprite = player:get_component(Sprite)
    local animation = player:get_component(Animation)

    local velocity = physics:get_linear_velocity()
    
    -- Better grounded check
    local isGrounded = math.abs(velocity.y) < 0.1
    
    -- Improved movement control
    local moveSpeed = 11800  -- Slightly reduced force for better control
    local horizontalInput = 0
    if Keyboard.pressed(KEY_A) then
        horizontalInput = -1
        if playerAnimationState ~= ANIMATION_STATE.RUN_LEFT then
            playerAnimationState = ANIMATION_STATE.RUN_LEFT
            animation.num_frames = 4
            animation.frame_offset = 2
        end
    elseif Keyboard.pressed(KEY_D) then
        horizontalInput = 1
        if playerAnimationState ~= ANIMATION_STATE.RUN_RIGHT then
            playerAnimationState = ANIMATION_STATE.RUN_RIGHT
            animation.num_frames = 4
            animation.frame_offset = 6
        end
    end
    
    -- Apply counter-force for smoother deceleration when changing directions
    if horizontalInput == 0 then
        -- Apply friction/damping when not pressing movement keys
        local dampingForce = -velocity.x * 500
        physics:apply_force_center(vec2(dampingForce, 0))
        if playerAnimationState ~= ANIMATION_STATE.IDLE then
            playerAnimationState = ANIMATION_STATE.IDLE
            animation.num_frames = 2
            animation.frame_offset = 0
        end
     
    elseif (horizontalInput < 0 and velocity.x > 0) or (horizontalInput > 0 and velocity.x < 0) then
        -- Counter force when changing directions - makes it feel more responsive
        local counterForce = -velocity.x * 1000
        physics:apply_force_center(vec2(counterForce, 0))
    end
    
    -- Apply movement force after counter-forces
    physics:apply_force_center(vec2(horizontalInput * moveSpeed, 0))
    
    -- Improved jump handling
    if Keyboard.just_pressed(KEY_W) and isGrounded then
        -- Small upward velocity reset before applying impulse (reduces inconsistency)
        physics:set_linear_velocity(vec2(velocity.x, 0))
        physics:linear_impulse(vec2(0, -6500))  -- Slightly reduced for better control
    end
    
    -- Better speed cap implementation
    local maxSpeed = 400
    velocity = physics:get_linear_velocity()  -- Get updated velocity after forces
    if math.abs(velocity.x) > maxSpeed then
        -- Smoother capping that preserves vertical velocity accurately
        physics:set_linear_velocity(vec2(maxSpeed * math.sign(velocity.x), velocity.y))
    end
end

function GameState:UpdateContacts()
	local uda, udb = ContactListener.get_user_data()
	if uda and udb then 
		gCollisionEvent:EmitEvent(uda, udb)
	end 
end
