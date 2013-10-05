#pragma once
#include <vector>
#include <memory>
#include "Entity/EntityParent.h"

class Entity;
class Level : public EntityParent
{
public:

private:

	std::vector<std::unique_ptr<Entity>> m_Entities;

};