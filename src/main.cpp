#include <entt/entt.hpp>
#include <iostream>

#include <agents/basic_agent.h>
#include <agents/components.h>

#include <command/basic_commander.h>
#include <command/components.h>

#include <movement/spatial_grid.h>

#include <simulation/simulation.h>

/**
 * @brief adds agents to registry
 * 
 * currently used to run quick tests,
 * in the future will be expanded to add agents based on scenario
 */
void addAgentsToRegistry(entt::registry &registry)
{
    create_unit(registry, &basic_commander, &basic_agent, 20);
}

int main()
{
    entt::registry registry;
    double simulation_time = 0.f;
    float time_step = 0.1f; // in seconds

    addAgentsToRegistry(registry);
    create_grids(registry, 5.f);

    int i = 0;
    while(true)
    {
        runSimulationStep(registry, time_step, simulation_time);
        
        registry.view<relationToCommander, position>().each([](const auto entity, const auto &relation, const auto &pos) {
            std::cout << "entity " << static_cast<unsigned>(entt::to_integral(entity))
                      << " parent-> "
                      << static_cast<unsigned>(entt::to_integral(relation.parent))
                      << " position -> " << pos.x << " " << pos.y << "\n";
            });
        i++;
        if(i > 4) break;
    }
    return 0;
}
