#pragma once

#include <iostream>
#include <sol/sol.hpp>
#include <entt/entt.hpp>

#include "../LOGGER/log.h"
#include "../PHYSICS/UserData.h"
#include "../ECS/MetaUtilities.h"

struct UserDataBinder
{
    static void CreateLuaUserData(sol::state& lua);

    template <typename DATA>
    static void register_meta_user_data();
};

template <typename DATA>
auto create_user_data(const sol::table& data, sol::this_state s);

template <typename DATA>
auto set_user_data(UserData* pUserData, const sol::table& data, sol::this_state s);

template <typename DATA>
auto get_user_data(UserData& userData, sol::this_state s);

#include "UserDataBinding.inl"
