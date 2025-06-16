#include "pch.h"

#include "ECS/COMPONENTS/AnimationComponent.h"

void AnimationComponent::CreateAnimationLuaBind( sol::state& lua )
{
	lua.new_usertype<AnimationComponent>(
		"Animation",
		"type_id", &entt::type_hash<AnimationComponent>::value,
		sol::call_constructor,
		sol::factories( []( int numFrames, int frameRate, int frameOffset, bool bVertical, bool bLooped ) {
			return AnimationComponent{
				.numFrames = numFrames,
				.frameRate = frameRate,
				.frameOffset = frameOffset,
				.bVertical = bVertical,
				.bLooped = bLooped };
		} ),
		"num_frames", &AnimationComponent::numFrames,
		"frame_rate", &AnimationComponent::frameRate,
		"frame_offset", &AnimationComponent::frameOffset,
		"current_frame", &AnimationComponent::currentFrame,
		"bVertical", &AnimationComponent::bVertical,
		"bLooped", &AnimationComponent::bLooped,
		"start_time", &AnimationComponent::startTime,
		"reset", []( AnimationComponent& anim ) {
			anim.currentFrame = 0;
			anim.startTime = glfwGetTime();
		} );
}
