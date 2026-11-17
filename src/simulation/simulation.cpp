#include <entt/entt.hpp>

#include <simulation/simulation.h>
#include <simulation/observation.h>
#include <simulation/decisions/basic_commander.h>
#include <movement/spatial_grid.h>
#include <agents/components.h>

/**
 * @brief creates observations that will be used by subsequent systems
 */
void createObservations(entt::registry &registry)
{
    update_observation_for_soldiers(registry);
    update_observation_for_commanders(registry);
}

/**
 * @brief sends orders that are due
 */
void sendOrders(entt::registry &registry, double simulation_time)
{
    update_current_order(registry, simulation_time);
}

/**
 * @brief calculates decisions made by commanders
 */
void calculateNewOrdersFromCommanders(entt::registry &registry, double simulation_time)
{
    calculate_new_order(registry, simulation_time);
}

/**
 * @brief calculates decisions made by soldiers
 */
void calculateSoldierDecisions(entt::registry &registry)
{

}

/**
 * @brief system that handles movement
 * current only updates position based on velocity
 */
void updatePositions(entt::registry &registry, float time_step)
{
    auto view = registry.view<position, const velocity>();
    
    for(auto [entity, pos, vel]: view.each()) {
        pos.x += vel.x * time_step;
        pos.y += vel.y * time_step;
    }

    update_grids(registry);
}

/**
 * @brief checks if enmies are still in range of the attacks
 */
void checkAttackRanges(entt::registry &registry)
{

}

/**
 * @brief updates agent states like Morale, Fatique
 */
void updateAgentStates(entt::registry &registry, float time_step)
{

}

/**
 * @brief collects metrics
 * collected metrics are used to determine if simulation is done and
 * to generate raports
 */
void collectMetrics(entt::registry &registry)
{

}

/**
 * @brief function that runs one step of simulation
 * goes over each phase of simulation
 */
void runSimulationStep(entt::registry &registry, float time_step, double &simulation_time)
{
    createObservations(registry);
    sendOrders(registry, simulation_time);
    calculateNewOrdersFromCommanders(registry, simulation_time);
    calculateSoldierDecisions(registry);
    updatePositions(registry, time_step);
    checkAttackRanges(registry);
    updateAgentStates(registry, time_step);
    collectMetrics(registry);

    simulation_time += time_step;
}
