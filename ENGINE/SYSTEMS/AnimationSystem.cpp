#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(Registry& registry) 
	:
	m_Registry{ registry }
{

}

void AnimationSystem::Update()
{
    auto view = m_Registry.GetRegistry().view<AnimationComponent, SpriteComponent, TransformComponent>();
    for (auto entity : view)
    {
        const auto& transform = view.get<TransformComponent>(entity);
        auto& sprite = view.get<SpriteComponent>(entity);
        auto& animation = view.get<AnimationComponent>(entity);

        // Calculate elapsed time in seconds
        double currentTime = glfwGetTime();

        // Calculate current frame based on time and frame rate
        // frameRate is in frames per second, so we multiply by time to get current frame
        animation.currentFrame = static_cast<int>((currentTime * animation.frameRate)) % animation.numFrames;

        // Update UV coordinates based on animation direction
        if (animation.bVertical)
        {
            // For vertical spritesheet, update v coordinate
            sprite.uvs.v = (animation.currentFrame * sprite.uvs.uv_height);
            sprite.uvs.u = (animation.frameOffset * sprite.uvs.uv_width);
        }
        else
        {
            // For horizontal spritesheet, update u coordinate
            sprite.uvs.u = (animation.currentFrame * sprite.uvs.uv_width) +
                (animation.frameOffset * sprite.uvs.uv_width * animation.numFrames);
        }
    }
}
