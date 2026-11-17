#include <float.h>
#include <entt/entt.hpp>

#include <movement/spatial_grid.h>
#include <simulation/observation.h>
#include <agents/components.h>
#include <agents/utils.h>
#include <command/components.h>

void update_observation_for_soldiers(entt::registry &registry)
{
    auto view = registry.view<
        closestEnemy,
        alliesInRadius,
        enemiesInRadius,
        const position,
        const perception,
        const side>();

    for(auto [entity, closest, allies, enemies, pos, perc, s]: view.each()) {
        closest.distance = FLT_MAX;
        allies.number = 0; allies.panicked = 0; allies.disabled = 0;
        enemies.number = 0; enemies.panicked = 0; enemies.disabled = 0;

        auto grids = registry.ctx().get<Grids>();
        
        grids.get_allies(s).query(pos, perc.allies_perception_radius, [&](entt::entity other) {
            if(entity == other) return;
            position other_pos = registry.get<position>(other);
            float distance = distance_pos(pos, other_pos);
            if(distance > perc.allies_perception_radius) return;
            status& stat = registry.get<status>(other);
            allies.number++;
            allies.panicked += stat==PANICKED;
            allies.disabled += stat==DISABLED;
        });
        
        // closest_enemy_search_radius is larger or equal to enemies_perception_radius
        // so we can iterate over a larger one one time
        grids.get_enemies(s).query(pos, perc.closest_enemy_search_radius, [&](entt::entity other) {
            if(entity == other) return;
            position other_pos = registry.get<position>(other);
            float distance = distance_pos(pos, other_pos);
            if(distance > perc.closest_enemy_search_radius) return;
            // update closest enemy (closest_enemy_search_radius)
            if(distance < closest.distance) {
                closest.distance = distance;
                closest.pos = other_pos;
            }
            // update enemies (enemies_perception_radius)
            if(distance < perc.enemies_perception_radius) {
                status stat = registry.get<status>(other);
                enemies.number++;
                enemies.panicked += stat==PANICKED;
                enemies.disabled += stat==DISABLED;
            }
        });
    }
}

void update_observation_for_commanders(entt::registry &registry)
{
    auto view = registry.view<
        unit,
        closestEnemyUnit,
        const position,
        const side>();
    
    auto view_other = registry.view<position, side>(entt::exclude<status>);
    
    // find closest enemy unit, and prepare unit component by setting most of it to zero
    for(auto [entity, u, enemy, pos, s]: view.each()) {
        u.can_fight = 0u; u.panicked = 0u; u.disabled = 0u;
        enemy.distance = FLT_MAX;
        for(auto [entity_other, pos_other, s_other]: view_other.each()) {
            if(s == s_other) continue;
            float distance = distance_pos(pos, pos_other);
            if(distance < enemy.distance) {
                enemy.distance = distance;
                enemy.pos = pos_other;
            }
        }
    }

    auto view_soldiers = registry.view<const status, const relationToCommander>();

    // update unit components
    for(auto [entity, stat, relation]: view_soldiers.each()) {
        auto& u = registry.get<unit>(relation.parent);
        u.can_fight += stat==FINE;
        u.panicked += stat==PANICKED;
        u.disabled += stat==DISABLED;
    }
}