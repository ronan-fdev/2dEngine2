#pragma once

#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <iostream>

#include "../LOGGER/log.h"
#include "../PHYSICS/ContactListener.h"
#include "../PHYSICS/UserData.h"
#include "../ECS/MetaUtilities.h"

class ContactListenerBind
{
private:
    static std::tuple<sol::object, sol::object> GetUserData(ContactListener& contactListener, sol::this_state s);

public:
    static void CreateLuaContactListener(sol::state& lua, entt::registry& registry);
};

