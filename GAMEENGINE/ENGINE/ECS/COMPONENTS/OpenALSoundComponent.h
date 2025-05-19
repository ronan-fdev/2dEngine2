#pragma once
#include <sol/sol.hpp>
#include <string>

struct OpenALSoundComponent
{
    /* OpenAL sound to play. */
    std::shared_ptr<class OpenALSound> pSound{ nullptr };

    static void CreateLuaOpenALSoundComponentBind(sol::state& lua);
};

