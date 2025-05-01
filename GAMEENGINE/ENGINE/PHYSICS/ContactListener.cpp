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

		try
		{
			auto a_any = std::any_cast<ObjectData>(a_data->userData);
			auto b_any = std::any_cast<ObjectData>(b_data->userData);

			a_any.AddContact(b_any);
			b_any.AddContact(a_any);

			a_data->userData.reset();
			a_data->userData = a_any;

			b_data->userData.reset();
			b_data->userData = b_any;

			SetUserContacts(a_data, b_data);
		}
		catch (const std::bad_any_cast& ex)
		{
			LOG_ERROR("Failed to cast user contacts: {}", ex.what());
			SetUserContacts(nullptr, nullptr);
		}
	}
}

void ContactListener::EndContact(b2WorldId myWorldId)
{
	b2ContactEvents contactEvents = b2World_GetContactEvents(myWorldId);
	for (int i = 0; i < contactEvents.endCount; ++i)
	{
		b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;
		UserData* a_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData(endEvent->shapeIdA)
				);
		UserData* b_data =
			reinterpret_cast<UserData*>(
				b2Shape_GetUserData(endEvent->shapeIdB)
				);

		try
		{
			auto a_any = std::any_cast<ObjectData>(a_data->userData);
			auto b_any = std::any_cast<ObjectData>(b_data->userData);

			if (!a_any.RemoveContact(b_any))
			{
				// TODO: LOG ERROR
			}

			if (!b_any.RemoveContact(a_any))
			{
				// TODO: LOG ERROR
			}

			a_data->userData.reset();
			a_data->userData = a_any;

			b_data->userData.reset();
			b_data->userData = b_any;

		}
		catch (const std::bad_any_cast& ex)
		{
			LOG_ERROR("Failed to cast user contacts: {}", ex.what());
		}

		// Use b2Shape_IsValid because a shape may have been destroyed
		if (b2Shape_IsValid(endEvent->shapeIdA) && b2Shape_IsValid(endEvent->shapeIdB))
		{
			//ShapesStopTouching(endEvent->shapeIdA, endEvent->shapeIdB);
			SetUserContacts(nullptr, nullptr);
		}
	}
}
