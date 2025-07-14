#include <iostream>
#include "action_manager.h"

using namespace std; 

int main() {
    // Steps per player, max contracts per trade, customer_max_size, max_contract_value, num_players
    Config config(10, 3, 3, 10, 5);
    ActionManager action_manager(config); 

    int timestep = 10;
    int input_action = 2450; 
    ActionVariant action = action_manager.RawToStructuredAction(timestep, input_action); 
    cout << action << endl; 
    cout << action_manager.StructuredToRawAction(action_manager.game_phase_of_timestep(timestep), action) << " " << input_action << endl; 
    return 0; 
}