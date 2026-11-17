#include <queue>

#include <entt/entt.hpp>

#include <agents/components.h>

/**
 * @brief component defining relation between soldier and commander
 * this component can be added to a soldier to assign him under commander
 */
struct relationToCommander
{
    entt::entity parent;
};

enum orderType
{
    NO_ORDER, HOLD, MOVE_TO, ENGAGE, WITHDRAW
};

/**
 * @brief component that keeps current order
 */
struct currentOrder
{
    orderType type;
    float x, y;
};

///////////////////////////////////
// components used by commanders //
///////////////////////////////////


struct order
{
    orderType type;
    float x, y;
    // absolute time since start of simulation that this order will be recieved at
    double receive_at;
};

/**
 * @brief component that keeps a list of orders waiting to be received by soldiers
 */
struct ordersQueue
{
    std::queue<order> orders;
};

//////////////////////////////////////////////
// components updated by observation system //
//////////////////////////////////////////////

/**
 * @brief keeps data about unit under commander
 * updated by observation phase in simulation
 */
struct unit
{
    const unsigned int total;
    unsigned int can_fight;
    unsigned int panicked;
    unsigned int disabled;
};

/**
 * @brief component that keeps information about closest unit
 */
struct closestEnemyUnit
{
    float distance;
    position pos;
};
