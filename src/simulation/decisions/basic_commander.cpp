#include <entt/entt.hpp>

#include <command/components.h>
#include <agents/components.h>


void update_current_order(entt::registry &registry, double simulation_time)
{
    auto view = registry.view<
        currentOrder,
        ordersQueue
    >();

    for(auto [entity, current, queue]: view.each()) {
        if(queue.orders.empty()) continue;
        order new_order = queue.orders.front();
        if(new_order.receive_at <= simulation_time) {
            queue.orders.pop();
            current = {new_order.type, new_order.x, new_order.y};
        }
    }
}

void calculate_new_order(entt::registry &registry, double simulation_time)
{
    auto view = registry.view<
        ordersQueue,
        const closestEnemyUnit
    >();

    // sensd order to engage nearest enemy
    for(auto [entity, queue, enemyUnit]: view.each()) {
        order new_order = {ENGAGE, enemyUnit.pos.x, enemyUnit.pos.y, simulation_time+1.0};
        queue.orders.push(new_order);
    }
}