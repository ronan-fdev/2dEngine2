#include "pch.h"

#include "UTILITIES/RandomGenerator.h"

void RandomGenerator::Initialize()
{
	m_RandomEngine.seed( std::random_device()() );
}

RandomGenerator::RandomGenerator()
	: RandomGenerator( 0, std::numeric_limits<uint32_t>::max() )
{
}

RandomGenerator::RandomGenerator( uint32_t min, uint32_t max )
	: m_RandomEngine{}, m_Distribution{ min, max }
{
	Initialize();
}

float RandomGenerator::GetFloat()
{
	return (float)m_Distribution( m_RandomEngine );
}

uint32_t RandomGenerator::GetInt()
{
	return m_Distribution( m_RandomEngine );
}
