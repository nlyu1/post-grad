#include <iostream>
#include "action_manager.h"

using namespace std; 

int main() {
    // Steps per player, max contracts per trade, customer_max_size, max_contract_value, num_players
    Config config(10, 5, 5, 30, 5);
    ActionManager action_manager(config); 

    for (int timestep = 0; timestep <= 10; ++timestep) {
        GamePhase phase = action_manager.game_phase_of_timestep(timestep);
        
        // Skip terminal phase
        if (phase == GamePhase::kTerminal) {
            cout << "Timestep " << timestep << ": Terminal phase, skipping" << endl;
            continue;
        }
        
        auto [min_action, max_action] = action_manager.valid_action_range(phase);
        
        cout << "Timestep " << timestep << " (Phase: " << static_cast<int>(phase) << "): Range [" << min_action << ", " << max_action << "]" << endl;
        
        int discrepancy_count = 0;
        for (int raw_action = min_action; raw_action <= max_action; ++raw_action) {
            try {
                ActionVariant structured_action = action_manager.RawToStructuredAction(timestep, raw_action);
                int reverse_raw_action = action_manager.StructuredToRawAction(phase, structured_action);
                
                if (raw_action != reverse_raw_action) {
                    cout << "  DISCREPANCY: Original=" << raw_action << " Reverse=" << reverse_raw_action 
                         << " Structured=" << structured_action << endl;
                    discrepancy_count++;
                }
            } catch (const std::exception& e) {
                cout << "  ERROR for action " << raw_action << ": " << e.what() << endl;
            }
        }
        
        if (discrepancy_count == 0) {
            cout << "  All " << (max_action - min_action + 1) << " actions consistent!" << endl;
        } else {
            cout << "  Found " << discrepancy_count << " discrepancies out of " << (max_action - min_action + 1) << " actions" << endl;
        }
        cout << endl;
    }
    
    return 0; 
}