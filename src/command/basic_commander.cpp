#include <queue>

#include <entt/entt.hpp>

#include <agents/components.h>

#include <command/basic_commander.h>
#include <command/components.h>

/**
 * @brief creates basic commander agent
 * @return created entity
 */
entt::entity basic_commander(entt::registry &registry, unsigned int count)
{
    const auto entity = registry.create();

    // shared with soldiers
    registry.emplace<position>(entity, 0.f, 0.f);
    registry.emplace<side>(entity, SIDE0);
    registry.emplace<agentType>(entity, BASIC_AGENT_TYPE);
    // specific to commanders
    registry.emplace<closestEnemyUnit>(entity, 0.f, position{0.f, 0.f});
    registry.emplace<unit>(entity, count, count, 0u, 0u);
    registry.emplace<ordersQueue>(entity, std::queue<order>());
    registry.emplace<currentOrder>(entity, NO_ORDER, 0.f, 0.f);

    return entity;
}

/**
 * @brief assignes soldier to commander
 * currently the relation is one-way from soldier to commander
 */
void assign(entt::registry &registry, entt::entity &soldier, entt::entity &commander)
{
    registry.emplace<relationToCommander>(soldier, commander);
}

/**
 * @brief simple function to create an unit with required relations
 * @param registry registry to which unit will be added
 * @param create_commander function that takes registry, count and returns entity
 * @param create_soldier function that takes registry and returns entity
 * @param count number of soldier in an unit
 */
void create_unit(entt::registry &registry,
                 std::function<entt::entity(entt::registry&, unsigned int)> create_commander,
                 std::function<entt::entity(entt::registry&)> create_soldier,
                 unsigned int count)
{
    auto commander = create_commander(registry, count);
    for (auto i = 0u; i < count; i++)
    {
        auto soldier = create_soldier(registry);
        assign(registry, soldier, commander);
    }
}