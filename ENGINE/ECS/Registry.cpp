#include "Registry.h"

Registry::Registry()
	: 
	m_pRegistry(nullptr)
{
	m_pRegistry = std::make_unique<entt::registry>();
}
