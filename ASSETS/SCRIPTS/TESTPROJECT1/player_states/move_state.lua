MoveState = {}
MoveState.__index = MoveState

function MoveState:Create(character)
	assert(character)
	local this = 
	{
		m_Character = character,
		m_EntityID = character.m_EntityID,
		m_Controller = character.m_Controller,
        m_PlayerState = character.m_bPlayerState,
		m_JumpSteps = 10,
		m_NumJumps = 0,
		m_bAttacking = false
	}

	local state = State("move")
	state:set_variable_table(this)
	state:set_on_enter( function(...) this:OnEnter(...) end )
	state:set_on_exit( function() this:OnExit() end)
	state:set_on_update( function(dt) this:OnUpdate(dt) end)
	state:set_on_render( function() end)

	setmetatable(this, self)
	return state
end

function MoveState:OnEnter(params) end
function MoveState:OnExit() end
function MoveState:OnUpdate(dt) 
	local player = Entity(self.m_EntityID)
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
        if self.m_Character.m_bPlayerState ~= ANIMATION_STATE.RUN_LEFT then
            self.m_Character.m_bPlayerState = ANIMATION_STATE.RUN_LEFT
            m_PlayerState = ANIMATION_STATE.RUN_LEFT
            animation.num_frames = 4
            animation.frame_offset = 2
        end
    elseif Keyboard.pressed(KEY_D) then
        horizontalInput = 1
        if self.m_Character.m_bPlayerState ~= ANIMATION_STATE.RUN_RIGHT then
            self.m_Character.m_bPlayerState = ANIMATION_STATE.RUN_RIGHT
            m_PlayerState = ANIMATION_STATE.RUN_RIGHT
            animation.num_frames = 4
            animation.frame_offset = 6
        end
    end
    
    -- Apply counter-force for smoother deceleration when changing directions
    if horizontalInput == 0 then
        -- Apply friction/damping when not pressing movement keys
        local dampingForce = -velocity.x * 500
        physics:apply_force_center(vec2(dampingForce, 0))
        if m_PlayerState ~= ANIMATION_STATE.IDLE then
            m_PlayerState = ANIMATION_STATE.IDLE
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