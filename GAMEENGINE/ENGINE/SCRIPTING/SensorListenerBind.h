#pragma once

#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <iostream>

#include "../LOGGER/log.h"
#include "../PHYSICS/SensorListener.h"
#include "../PHYSICS/UserData.h"
#include "../ECS/MetaUtilities.h"

class SensorListenerBind
{
private:
    static std::tuple<sol::object, sol::object> GetUserData(SensorListener& sensorListener, sol::this_state s);

public:
    static void CreateLuaContactListener(sol::state& lua, entt::registry& registry);
};

