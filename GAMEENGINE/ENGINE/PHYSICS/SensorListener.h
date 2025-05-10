#pragma once

#include <iostream>

#include "Box2DWrappers.h"
#include "UserData.h"
#include "../LOGGER/log.h"

class SensorListener
{
private:
    UserData* m_pUserSensorDataA{ nullptr };
    UserData* m_pUserSensorDataB{ nullptr };

    void SetUserSensorContacts(UserData* a, UserData* b);

public:
    void BeginSensorContact(b2WorldId myWorldId);
    void EndSensorContact(b2WorldId myWorldId);

    //void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
    //void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

    UserData* GetUserSensorDataA() { return m_pUserSensorDataA; }
    UserData* GetUserSensorDataB() { return m_pUserSensorDataB; }

    void ResetUserSensorAB();
};

