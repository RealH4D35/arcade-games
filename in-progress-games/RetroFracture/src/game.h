#ifndef GAME_H
#define GAME_H

#include "splashkit.h"
#include "player.h"
#include "hud.h"
#include "hallwaylevel.h"
#include "lvl1.h"

/**
 * Main game controller class
 * Manages game state, levels, camera, and input handling
 */
class Game 
{
private:
    Player player;                      // Player character
    bool quit_game;                     // Game exit flag
    
    // Level tracking enumeration
    enum CurrentLevel 
    {
        LEVEL_HALLWAY,                  // Hallway level
        LEVEL_APARTMENT_101             // Apartment level
    };
    
    CurrentLevel current_level;         // Current active level
    point_2d camera_pos;                // Camera position
    
    // Level instances
    HallwayLevel hallway;               // Hallway level instance
    Level1 apartment101;                // Apartment level instance
    
    HUD hud;                            // Heads-up display
    std::string nearby_interaction_prompt; // Current interaction prompt text

public:
    Game();                             // Constructor
    
    void run();                         // Main game loop
    void update();                      // Update game state
    void draw();                        // Render game
    void handle_input();                // Process user input
    
    // Level management
    void enter_hallway();               // Transition to hallway level
    void enter_apartment101();          // Transition to apartment level
    
    // Camera management
    void update_camera();               // Update camera position
    point_2d world_to_screen(point_2d world_pos) const; // Convert coordinates
    
    // Getters
    float get_current_ground_level() const;         // Get current level's ground height
    float get_current_background_y_offset() const;  // Get background offset
    bool is_in_hallway() const { return current_level == LEVEL_HALLWAY; } // Level check
};

#endif