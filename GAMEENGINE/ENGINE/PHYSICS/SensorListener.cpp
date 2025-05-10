#include "SensorListener.h"

void SensorListener::SetUserSensorContacts(UserData* a, UserData* b)
{
	m_pUserSensorDataA = a;
	m_pUserSensorDataB = b;
}

void SensorListener::BeginSensorContact(b2WorldId myWorldId)
{
	b2SensorEvents sensorEvents = b2World_GetSensorEvents(myWorldId);
	for (int i = 0; i < sensorEvents.beginCount; ++i)
	{
		b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
		if (b2Shape_IsValid(beginTouch->sensorShapeId) && b2Shape_IsValid(beginTouch->visitorShapeId))
		{
			UserData* a_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData(beginTouch->sensorShapeId)
				);
			UserData* b_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData(beginTouch->visitorShapeId)
				);
			if (a_data && b_data)
			{
				SetUserSensorContacts(a_data, b_data);
			}
		}
	}
}

void SensorListener::EndSensorContact(b2WorldId myWorldId)
{
	b2SensorEvents sensorEvents = b2World_GetSensorEvents(myWorldId);
	for (int i = 0; i < sensorEvents.endCount; ++i)
	{
		b2SensorEndTouchEvent* endTouch = sensorEvents.endEvents + i;
		if (b2Shape_IsValid(endTouch->sensorShapeId) && b2Shape_IsValid(endTouch->visitorShapeId))
		{
			UserData* a_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData(endTouch->sensorShapeId)
				);
			UserData* b_data = reinterpret_cast<UserData*>(
				b2Shape_GetUserData(endTouch->visitorShapeId)
				);
			SetUserSensorContacts(nullptr, nullptr);
		}
	}
}

void SensorListener::ResetUserSensorAB()
{
	SetUserSensorContacts(nullptr, nullptr);
}
