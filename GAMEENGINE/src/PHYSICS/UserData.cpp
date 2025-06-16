#include "pch.h"

#include "PHYSICS/UserData.h"

bool ObjectData::AddContact( const ObjectData* objectData )
{
	auto contactItr = std::find_if(
		contactEntities.begin(), contactEntities.end(),
		[&]( const ObjectData* contactInfo ) {
			return *contactInfo == *objectData;
		} );

	if ( contactItr != contactEntities.end() )
		return false;

	contactEntities.push_back( objectData );
	// std::cout << contactEntities.size() << std::endl;

	return true;
}

bool ObjectData::RemoveContact( const ObjectData* objectData )
{
	auto contactItr = std::remove_if(
		contactEntities.begin(), contactEntities.end(),
		[&]( const ObjectData* contactInfo ) {
			return *contactInfo == *objectData;
		} );

	if ( contactItr == contactEntities.end() )
		return false;

	contactEntities.erase( contactItr, contactEntities.end() );
	// std::cout << contactEntities.size() << std::endl;
	return true;
}

std::string ObjectData::to_string() const
{
	std::stringstream ss;
	ss << "==== Object Data ==== \n"
	   << std::boolalpha << "Tag: " << tag << "\n"
	   << "Group: " << group << "\n"
	   << "bCollider: " << bCollider << "\n"
	   << "bTrigger: " << bTrigger << "\n"
	   << "bSensor: " << bSensor << "\n"
	   << "EntityID: " << entityID << "\n";

	return ss.str();
}

bool operator==( const ObjectData& a, const ObjectData& b )
{
	return a.bCollider == b.bCollider &&
		   a.bTrigger == b.bTrigger &&
		   a.bSensor == b.bSensor &&
		   a.tag == b.tag &&
		   a.group == b.group &&
		   a.entityID == b.entityID;
}
