#include "Entity/EntityFactory.h"
#include "Entity/Entity.h"
#include "UI/InputManager.h"
#include "Message/SpawnEntityMessage.h"
#include "Component/FlyingAiComponent.h"
#include "Component/LocationComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/SoundComponent.h"
#include "Component/InputComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/FactionComponent.h"
#include "Component/HealthComponent.h"
#include "Component/PhysicsComponent.h"
#include "Message/MessageFactory.h"
#include "Message/ScriptMessageInterface.h"
#include "VM/Core/VmState.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/Compiler/Compiler.h"

#include <stdexcept>
#include "Utility/LoggerManager.h"
#include "Utility/GenericDefines.h"


// sobadsobadsobad
// honestly, whole thing should be defined in a data file somewhere


void RegisterNativeBindings(VMState &state) {
  
  void(*logger)(VMValue) = [](VMValue value) -> void
  {
    LoggerManager::GetLog(VM_LOG).AddLine(LogLevel::INFO, "Debug print: " + value.ToString());
  };


  int(*rand_ptr)(int, int) = [](int low, int high) -> int
  { 
    auto val = (rand() % (high - low)) + low;
    return val;
  };

  auto randBinding = CreateBinding(rand_ptr);

  state.AddNativeBinding("log", CreateBinding(logger));
  state.AddNativeBinding("RegisterMessageHandler", CreateBinding(&Entity::RegisterScriptMessageHandler));
  state.AddNativeBinding("SendSpawnEntityMessage", CreateBinding(&ScriptMessageInterface::SendSpawnMessage));
  state.AddNativeBinding("SendBlinkMessage", CreateBinding(&ScriptMessageInterface::SendBlinkingMessage));
  state.AddNativeBinding("SendAddHealthMessage", CreateBinding(&ScriptMessageInterface::SendAddHealthMessage));
  state.AddNativeBinding("SendTakeDamageMessage", CreateBinding(&ScriptMessageInterface::SendTakeDamageMessage));
  state.AddNativeBinding("SendTerminateEntityMessage", CreateBinding(&ScriptMessageInterface::SendTerminateEntityMessage));
  state.AddNativeBinding("SendPlaySoundEffectMessage", CreateBinding(&ScriptMessageInterface::SendPlaySoundEffectMessage));
  state.AddNativeBinding("SendVisibilityMessage", CreateBinding(&ScriptMessageInterface::SendVisibilityMessage));
  state.AddNativeBinding("SendImportantCharacterDeathMessage", CreateBinding(&ScriptMessageInterface::SendImportantCharacterDeathMessage));
  state.AddNativeBinding("SendEndLevelMessage", CreateBinding(&ScriptMessageInterface::SendEndLevelMessage));
  state.AddNativeBinding("SendMessageUpwards", CreateBinding(&Entity::SendMessageUpwards));
  
  state.AddNativeBinding("GetEntityFaction", CreateBinding(&ScriptMessageInterface::GetFaction));

  state.AddNativeBinding("CollisionMessageGetEntities", CreateBinding(&ScriptMessageInterface::CollisionMessageGetFactions));
  state.AddNativeBinding("BlinkMessageGetDuration", CreateBinding(&ScriptMessageInterface::BlinkMessageGetDuration));
  state.AddNativeBinding("GetHitType", CreateBinding(&ScriptMessageInterface::GetHitType));

  state.AddNativeBinding("Rand", randBinding);
}


void CreateBullet(Entity *e, SpawnEntityMessage *msg)
{
  const int bulletHitboxSize = 15;
  Entity *spawner = msg->Spawner();

  LocationComponent *spawnerLocation = static_cast<LocationComponent *>(spawner->GetComponent(ComponentType::LOCATION));
  FactionComponent *spawnerFaction = static_cast<FactionComponent *>(spawner->GetComponent(ComponentType::FACTION));
  SDL_assert(spawnerLocation != nullptr);

  Faction bulletFaction = Faction::NONE;
  if (spawnerFaction != nullptr)
  {
    bulletFaction = spawnerFaction->GetFaction();
  }

  
  double x = spawnerLocation->GetX();
  double y = spawnerLocation->GetY();
  double speed = 10.0;
  double xVel = 0;
  double yVel = 0;
  
  Direction direction = spawnerLocation->GetFiringDirection();

  switch (direction)
  {
    case Direction::RIGHT:
       x += bulletHitboxSize;
       xVel = speed;
      break;
    case Direction::LEFT:
      x -= bulletHitboxSize;
      xVel = -speed;
      break;
    case Direction::UP:
      y -= bulletHitboxSize;
      yVel = -speed;
      break;
    case Direction::DOWN:
      y += bulletHitboxSize;
      yVel = speed;
      break;
  default:
    break;
  }
  
  std::unique_ptr<LocationComponent> location(new LocationComponent());
  location->SetLocation(x, y);
  location->SetDirection(direction);

  std::unique_ptr<GraphicsComponent> graphics(new GraphicsComponent());
  graphics->AddFrame(0, 200007);

  std::unique_ptr<CollisionComponent> collision(new CollisionComponent);
  collision->AddHitbox(8, 8, bulletHitboxSize, bulletHitboxSize, HitboxType::TRIGGER);
  std::unique_ptr<VelocityComponent> velocity(new VelocityComponent(0, 0));
    
  velocity->SetVelocity(xVel, yVel);

  std::unique_ptr<FactionComponent> faction(new FactionComponent(bulletFaction));
  std::unique_ptr<SoundComponent> sound(new SoundComponent());
  sound->AddSoundEffect(SoundEffectType::DEATH, SOUND_GUN_HIT);

  e->AddComponent(ComponentType::SOUND, std::move(sound));
  e->AddComponent(ComponentType::COLLISION, std::move(collision));
  e->AddComponent(ComponentType::GRAPHICS, std::move(graphics));
  e->AddComponent(ComponentType::LOCATION, std::move(location));
  e->AddComponent(ComponentType::VELOCITY, std::move(velocity));
  e->AddComponent(ComponentType::FACTION, std::move(faction));  

  VMState damageColliderScript = Compiler::Compile("data/scripts/DamageColliderScript.txt");
  RegisterNativeBindings(damageColliderScript);
  e->AddVmScript(std::move(damageColliderScript));

  VMState bulletScript = Compiler::Compile("data/scripts/BulletScript.txt");
  RegisterNativeBindings(bulletScript);
  e->AddVmScript(std::move(bulletScript));
}

void CreateHealthPickup(Entity *e, SpawnEntityMessage *msg)
{
  const int hitboxSize = 43;
  Entity *spawner = msg->Spawner();

  LocationComponent *spawnerLocation = static_cast<LocationComponent *>(spawner->GetComponent(ComponentType::LOCATION));
 
  double x = spawnerLocation->GetX();
  double y = spawnerLocation->GetY();

  e->AddComponent(ComponentType::LOCATION, std::unique_ptr<Component>(new LocationComponent(x, y)));
  e->AddComponent(ComponentType::VELOCITY, std::unique_ptr<Component>(new VelocityComponent(0, 0)));
  e->AddComponent(ComponentType::PHYSICS, std::unique_ptr<Component>(new PhysicsComponent));
  
  std::unique_ptr<GraphicsComponent> graphics(new GraphicsComponent());
  
  graphics->AddFrame(0, 400000);

  std::unique_ptr<CollisionComponent> collision(new CollisionComponent);
  collision->AddHitbox(0, 0, hitboxSize, hitboxSize, HitboxType::TRIGGER);
  

  e->AddComponent(ComponentType::COLLISION, std::move(collision));
  e->AddComponent(ComponentType::GRAPHICS, std::move(graphics));

  VMState healCollider = Compiler::Compile("data/scripts/HealColliderScript.txt");
  RegisterNativeBindings(healCollider);
  e->AddVmScript(std::move(healCollider));
}


std::unique_ptr<Entity> EntityFactory::CreatePlayer(int x, int y, InputManager &input)
{
  std::unique_ptr<Entity> e(new Entity);

  VMState invulnerabilityOnHitScript = Compiler::Compile("data/scripts/InvulnerabilityOnHitScript.txt");
  RegisterNativeBindings(invulnerabilityOnHitScript);
  e->AddVmScript(std::move(invulnerabilityOnHitScript));

  VMState blinkScript = Compiler::Compile("data/scripts/BlinkScript.txt");
  RegisterNativeBindings(blinkScript);
  e->AddVmScript(std::move(blinkScript));

  VMState quitGameOnDeathScript = Compiler::Compile("data/scripts/QuitGameOnDeathScript.txt");
  RegisterNativeBindings(quitGameOnDeathScript);
  e->AddVmScript(std::move(quitGameOnDeathScript));


  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<VelocityComponent> v(new VelocityComponent(5, 13));
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::PLAYER));
  std::unique_ptr<PhysicsComponent> p(new PhysicsComponent);
  std::unique_ptr<InputComponent> i(new InputComponent);
  std::unique_ptr<HealthComponent> h(new HealthComponent(4, 4, 3));
  i->RegisterInputHandler(input);

  g->AddFrame(IDLE_ANIMATION, 200002, 20);
  g->AddFrame(WALKING_ANIMATION, 200002, 4);
  g->AddFrame(WALKING_ANIMATION, 200002 + 1, 4);
  g->AddFrame(WALKING_ANIMATION, 200002 + 2, 4);
  
  std::unique_ptr<SoundComponent> sound(new SoundComponent());
  
  sound->AddSoundEffect(SoundEffectType::TAKE_DAMAGE, SOUND_TAKE_DAMAGE);
  sound->AddSoundEffect(SoundEffectType::JUMP, SOUND_JUMP);
  sound->AddSoundEffect(SoundEffectType::SHOOT, SOUND_GUN_SHOOT);

  e->AddComponent(ComponentType::SOUND, std::move(sound));

  e->AddComponent(ComponentType::GRAPHICS, std::move(g));
  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  e->AddComponent(ComponentType::VELOCITY, std::move(v));
  e->AddComponent(ComponentType::INPUT, std::move(i));
  
  c->AddHitbox(0, 0, 64, 90, HitboxType::SOLID);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  e->AddComponent(ComponentType::FACTION, std::move(f));
  e->AddComponent(ComponentType::PHYSICS,std::move(p));
  e->AddComponent(ComponentType::HEALTH, std::move(h));
  
  return e;
}

std::unique_ptr<Entity> BaseEnemy(int x, int y)
{
  std::unique_ptr<Entity> e(new Entity);
  e->AddComponent(ComponentType::COLLISION,std::unique_ptr<CollisionComponent>(new CollisionComponent));
  e->AddComponent(ComponentType::LOCATION,  std::unique_ptr<LocationComponent>(new LocationComponent(x, y)));
  e->AddComponent(ComponentType::GRAPHICS, std::unique_ptr<GraphicsComponent>(new GraphicsComponent));
  e->AddComponent(ComponentType::VELOCITY, std::unique_ptr<VelocityComponent>(new VelocityComponent(5, 5)));
  e->AddComponent(ComponentType::HEALTH, std::unique_ptr<Component>(new HealthComponent(3, 3, 0)));
  e->AddComponent(ComponentType::INPUT,  std::unique_ptr<InputComponent>(new InputComponent(-1)));
  e->AddComponent(ComponentType::FACTION, std::unique_ptr<FactionComponent>(new FactionComponent(Faction::ENEMY)));

  VMState damageCollider = Compiler::Compile("data/scripts/DamageColliderScript.txt");
  RegisterNativeBindings(damageCollider);

  e->AddVmScript(std::move(damageCollider));
  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateFlyingEnemy(int x, int y, Entity *target)
{
  
  auto e = BaseEnemy(x, y);
  e->AddComponent(ComponentType::AI, std::unique_ptr<FlyingAiComponent>(new FlyingAiComponent(target)));

  auto g = static_cast<GraphicsComponent *>(e->GetComponent(ComponentType::GRAPHICS));
  auto c = static_cast<CollisionComponent *>(e->GetComponent(ComponentType::COLLISION));
  g->AddFrame(0, 200028, 5);
  g->AddFrame(0, 200032, 5);

  c->AddHitbox(0,0,70,35, HitboxType::SOLID);
  c->AddHitbox(0,0,70,35, HitboxType::TRIGGER);

  std::unique_ptr<SoundComponent> sound(new SoundComponent());
  sound->AddSoundEffect(SoundEffectType::DEATH, SOUND_MONSTER_DEATH);

  e->AddComponent(ComponentType::SOUND, std::move(sound));
  

  auto spawnHealthPickupOnDeathScript = Compiler::Compile("data/scripts/SpawnHealthPickupOnDeathScript.txt");
  RegisterNativeBindings(spawnHealthPickupOnDeathScript);

  e->AddVmScript(std::move(spawnHealthPickupOnDeathScript));
  

  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateBlock(int x, int y, int tileid)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);


  g->AddFrame(0, tileid); 
  e->AddComponent(ComponentType::GRAPHICS, std::move(g));

  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));

  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateCollisionBlock(int x, int y, int w, int h)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());


  l->SetLocation(x, y);
  c->AddHitbox(0, 0, w, h, HitboxType::SOLID);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  return e;

}



std::unique_ptr<Entity> EntityFactory::CreateEndLevelEntity(int x, int y)
{

  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  g->AddFrame(0, 400006);
  e->AddComponent(ComponentType::GRAPHICS,std::move(g));

  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  c->AddHitbox(0, 0, 70, 71,HitboxType::TRIGGER);
  e->AddComponent(ComponentType::COLLISION, std::move(c));

  e->AddComponent(ComponentType::LOCATION, std::unique_ptr<LocationComponent>(new LocationComponent(x, y)));


  auto endLevelScript = Compiler::Compile("data/scripts/EndLevelOnCollisionScript.txt");
  RegisterNativeBindings(endLevelScript);
  e->AddVmScript(std::move(endLevelScript));

  return e;
}


std::unique_ptr<Entity> EntityFactory::CreateEntity(SpawnEntityMessage *msg)
{
  std::unique_ptr<Entity> e(new Entity);

  switch (msg->GetEntityType())
  {
  case EntityType::BULLET:
    CreateBullet(e.get(), msg);
    break;
  case EntityType::HEALTH_PICKUP:
    CreateHealthPickup(e.get(), msg);
    break;
  default:
    throw std::runtime_error("Default case reached in EntityFactory::CreateEntity() reached");
    break;
  }
  return e;

}
