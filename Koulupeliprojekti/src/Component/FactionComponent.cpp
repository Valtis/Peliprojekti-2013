#include "Component/FactionComponent.h"

FactionComponent::FactionComponent() : m_faction(Faction::NONE)
{

}

FactionComponent::FactionComponent(Faction f) : m_faction(f)
{

}

FactionComponent::~FactionComponent()
{

}