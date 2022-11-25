#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent& event) {
        Entity a = event.a;
        Entity b = event.b;
        Logger::Log("Collision event emitted: " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));

        if (a.BelongsToGroup("projectiles") && b.HasTag("player")) {
            OnProjectileHitsPlayer(a, b); //a is the projectile and b is the player
        }

        if (b.BelongsToGroup("projectiles") && a.HasTag("player")) {
            OnProjectileHitsPlayer(b, a); //b is the projectile and a is the player
        }

        if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies")) {
            OnProjectileHitsEnemy(a, b); //a is the projectile and b is the enemy
        }

        if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies")) {
            OnProjectileHitsEnemy(b, a); //b is the projectile and a is the enemy
        }
    }

    void OnProjectileHitsEntity(Entity projectile, Entity entity) {
        const auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();
        auto& health = entity.GetComponent<HealthComponent>();
        health.healthPercentage -= projectileComponent.hitPercentDamage;

        if (health.healthPercentage <= 0) {
            entity.Kill();
        }
        projectile.Kill();
    }

    void OnProjectileHitsPlayer(Entity projectile, Entity player) {
        const auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly) {
            auto& health = player.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentDamage;
            Logger::Err("The health of " + std::to_string(player.GetId()) + "is now: " + std::to_string(health.healthPercentage));

            if (health.healthPercentage <= 0) {
                player.Kill();
            }
            projectile.Kill();
        }
    }

    void OnProjectileHitsEnemy(Entity projectile, Entity enemy) {
        const auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (projectileComponent.isFriendly) {
            auto& health = enemy.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentDamage;
            Logger::Err("The health of " + std::to_string(enemy.GetId()) + "is now: " + std::to_string(health.healthPercentage));

            if (health.healthPercentage <= 0) {
                enemy.Kill();
            }
            projectile.Kill();
        }
    }

    void Update() {
        // TODO:...
    }
};

#endif
