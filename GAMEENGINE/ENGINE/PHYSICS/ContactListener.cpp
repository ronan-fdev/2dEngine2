#include "ContactListener.h"

void ContactListener::SetUserContacts(UserData* a, UserData* b)
{
	m_pUserDataA = a;
	m_pUserDataB = b;
}

void ContactListener::BeginContact(b2WorldId myWorldId)
{
	b2ContactEvents contactEvents = b2World_GetContactEvents(myWorldId);
	for (int i = 0; i < contactEvents.beginCount; ++i)
	{
		b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
		UserData* a_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData(beginEvent->shapeIdA)
				);
		UserData* b_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData(beginEvent->shapeIdB)
				);

		SetUserContacts(a_data, b_data);
	}
}

void ContactListener::EndContact(b2WorldId myWorldId)
{
	b2ContactEvents contactEvents = b2World_GetContactEvents(myWorldId);
	for (int i = 0; i < contactEvents.endCount; ++i)
	{
		b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;

		// Use b2Shape_IsValid because a shape may have been destroyed
		if (b2Shape_IsValid(endEvent->shapeIdA) && b2Shape_IsValid(endEvent->shapeIdB))
		{
			//ShapesStopTouching(endEvent->shapeIdA, endEvent->shapeIdB);
			SetUserContacts(nullptr, nullptr);
		}
	}
}
