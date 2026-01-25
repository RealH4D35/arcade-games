#include "game.h"
#include <iostream>

/**
 * Game constructor
 * Initializes game state and starts in hallway level
 */
Game::Game() : 
    quit_game(false), 
    current_level(LEVEL_HALLWAY),
    camera_pos(point_at(0, 0)),
    nearby_interaction_prompt("")
{
    // Initialize game in hallway level
    enter_hallway();
}

/**
 * Transition to hallway level
 * Resets player position and camera for hallway
 */
void Game::enter_hallway() 
{
    current_level = LEVEL_HALLWAY;
    
    // Load hallway assets
    hallway.load();
    
    // Position player at hallway start (near the lobby exit)
    const float START_X = 1435.0f;  // Position of "hexit" (Exit To Lobby)
    player.set_position(point_at(START_X, hallway.get_ground_level()));
    player.set_grounded(true);
    player.set_state(STATE_IDLE);
    
    // Reset camera with left boundary clamping
    camera_pos = point_at(START_X - 400.0f, hallway.get_ground_level() - 300.0f);
    if (camera_pos.x < 0) camera_pos.x = 0;
    if (camera_pos.y < -100.0f) camera_pos.y = -100.0f; // Prevent excessive negative Y
    
    // Clear interaction prompt
    nearby_interaction_prompt = "";
    
    write_line("Entered hallway level");
}

/**
 * Transition to apartment level
 * Resets player position and camera for apartment
 */
void Game::enter_apartment101() 
{
    current_level = LEVEL_APARTMENT_101;
    apartment101.reset_door();  // Reset apartment door state
    
    // Position player at apartment door position
    // Using the same X position as the exit door in the apartment level
    const float START_X = 100.0f;  // This matches the exit_door X position in lvl1.cpp
    player.set_position(point_at(START_X, apartment101.get_ground_level()));
    player.set_grounded(true);
    player.set_state(STATE_IDLE);
    
    // Reset camera (no horizontal clamping in apartment)
    camera_pos = point_at(START_X - 400.0f, apartment101.get_ground_level() - 300.0f);
    
    // Clear interaction prompt
    nearby_interaction_prompt = "";
    
    write_line("Entered Apartment 101 (Level 1)");
}

/**
 * Main game loop
 * Runs at 60 FPS until quit is requested
 */
void Game::run() 
{
    while (!quit_game && !quit_requested()) 
    {
        process_events();
        handle_input();
        update();
        draw();
        refresh_screen(60);  // Cap at 60 FPS
    }
}

/**
 * Update game state
 * Handles player updates, camera movement, and level-specific logic
 */
void Game::update() 
{
    // Update player physics and animation
    player.update();
    update_camera();
    
    // Clear interaction prompt first
    nearby_interaction_prompt = "";
    
    // Level-specific updates
    switch (current_level) 
    {
        case LEVEL_HALLWAY:
            hallway.check_boundaries(player);
            hallway.check_interactions(player);
            
            // Update interaction prompt for nearby objects
            {
                Interactable* nearby = hallway.get_nearby_interactable();
                if (nearby != nullptr && !nearby->get_is_triggered()) 
                {
                    nearby_interaction_prompt = "Press E to enter " + nearby->get_label();
                }
            }
            break;
            
        case LEVEL_APARTMENT_101:
            apartment101.check_boundaries(player);
            apartment101.check_interactions(player);
            
            // Update interaction prompt for exit door
            if (apartment101.is_door_triggered()) 
            {
                nearby_interaction_prompt = "Press E to exit to hallway";
            }
            break;
    }
}

/**
 * Update camera position with smooth following
 * Applies different clamping rules per level
 */
void Game::update_camera() 
{
    const float CAMERA_LERP_FACTOR = 0.1f;  // Smoothing factor
    point_2d player_pos = player.get_position();
    
    // Target camera position (centered on player)
    float target_camera_x = player_pos.x - 400.0f;
    float target_camera_y = player_pos.y - 300.0f;
    
    // Smooth camera movement using linear interpolation
    camera_pos.x = camera_pos.x * (1.0f - CAMERA_LERP_FACTOR) + target_camera_x * CAMERA_LERP_FACTOR;
    camera_pos.y = camera_pos.y * (1.0f - CAMERA_LERP_FACTOR) + target_camera_y * CAMERA_LERP_FACTOR;
    
    // Apply level-specific camera constraints
    if (current_level == LEVEL_HALLWAY) 
    {
        // Prevent camera from going left of hallway
        if (camera_pos.x < 0) 
        {
            camera_pos.x = 0;
        }
        
        // Prevent camera from going too high (show ceiling)
        float min_camera_y = hallway.get_ground_level() - 450.0f; // Adjust as needed
        if (camera_pos.y < min_camera_y) 
        {
            camera_pos.y = min_camera_y;
        }
    }
    
    // General camera constraints (apply to all levels)
    // Prevent camera from going too high
    float max_camera_y = get_current_ground_level() - 200.0f; // Keep some sky visible
    if (camera_pos.y > max_camera_y) 
    {
        camera_pos.y = max_camera_y;
    }
    
    // Prevent excessive negative camera Y
    float min_camera_y_general = -100.0f; // Adjust as needed
    if (camera_pos.y < min_camera_y_general) 
    {
        camera_pos.y = min_camera_y_general;
    }
}

/**
 * Convert world coordinates to screen coordinates
 * @param world_pos World position to convert
 * @return Corresponding screen position
 */
point_2d Game::world_to_screen(point_2d world_pos) const 
{
    float background_y_offset = get_current_background_y_offset();
    return point_at(
        world_pos.x - camera_pos.x, 
        world_pos.y - camera_pos.y + background_y_offset
    );
}

/**
 * Get current level's ground level
 * @return Ground Y-coordinate for current level
 */
float Game::get_current_ground_level() const 
{
    switch (current_level) 
    {
        case LEVEL_HALLWAY: 
            return hallway.get_ground_level();
        case LEVEL_APARTMENT_101: 
            return apartment101.get_ground_level();
        default: 
            return 500.0f;  // Fallback default
    }
}

/**
 * Get current level's background Y offset
 * @return Background offset for current level
 */
float Game::get_current_background_y_offset() const 
{
    switch (current_level) 
    {
        case LEVEL_HALLWAY: 
            return hallway.get_y_offset();
        case LEVEL_APARTMENT_101: 
            return 0.0f;  // No offset for apartment
        default: 
            return 0.0f;
    }
}

/**
 * Render game frame
 * Draws level background, player, and HUD elements
 */
void Game::draw() 
{
    // Draw level-specific background
    switch (current_level) 
    {
        case LEVEL_HALLWAY:
            hallway.draw(camera_pos);
            break;
            
        case LEVEL_APARTMENT_101:
            apartment101.draw(player, camera_pos);
            break;
    }
    
    // Draw player character
    point_2d screen_pos = world_to_screen(player.get_position());
    player.draw_at(screen_pos);
    
    // Determine if player is in hallway for HUD rendering
    bool in_hallway = is_in_hallway();
    
    // Draw all HUD elements
    hud.draw_controls(in_hallway);
    hud.draw_debug_info(player, camera_pos, in_hallway);
    
    // Draw level name
    std::string level_name = in_hallway ? "Apartment Hallway" : "Apartment 101";
    hud.draw_level_info(level_name, in_hallway);
    
    // Draw interaction prompt if available
    hud.draw_interaction_prompt(nearby_interaction_prompt, in_hallway);
}

/**
 * Handle user input
 * Processes keyboard input for movement, interactions, and game control
 */
void Game::handle_input() 
{
    bool in_hallway = is_in_hallway();
    
    // --- Player Movement ---
    if (key_down(A_KEY)) 
    {
        if (in_hallway) 
        {
            // In hallway: check left boundary
            point_2d pos = player.get_position();
            const float SPRITE_HALF_WIDTH = 48.0f;
            if (pos.x > SPRITE_HALF_WIDTH) 
            {
                player.move_left();
            }
        } 
        else 
        {
            // In apartment: no left boundary
            player.move_left();
        }
    }
    else if (key_down(D_KEY)) 
    {
        player.move_right();
    }
    else 
    {
        player.stop_moving();
    }
    
    // Jump input
    if (key_typed(W_KEY)) 
    {
        player.jump();
    }
    
    // --- Interaction Input ---
    if (key_typed(E_KEY)) 
    {
        if (in_hallway) 
        {
            Interactable* nearby = hallway.get_nearby_interactable();
            if (nearby != nullptr && !nearby->get_is_triggered()) 
            {
                hallway.trigger_interaction(nearby->get_id());
                
                // Handle specific door interactions
                if (nearby->get_id() == "door1") 
                {
                    enter_apartment101();
                } 
                else if (nearby->get_id() == "door2") 
                {
                    write_line("Apartment 102 (coming soon!)");
                    // Future: enter_apartment102();
                }
                else if (nearby->get_id() == "door3") 
                {
                    write_line("Apartment 103 (coming soon!)");
                    // Future: enter_apartment103();
                }
                else if (nearby->get_id() == "door5") 
                {
                    write_line("Apartment 105 (coming soon!)");
                    // Future: enter_apartment105();
                }
                else if (nearby->get_id() == "door6") 
                {
                    write_line("Apartment 106 (coming soon!)");
                    // Future: enter_apartment106();
                }
                else if (nearby->get_id() == "door7") 
                {
                    write_line("Apartment 107 (coming soon!)");
                    // Future: enter_apartment107();
                }
                else if (nearby->get_id() == "hexit") 
                {
                    write_line("Exit to lobby (coming soon!)");
                    // Future: enter_lobby();
                }
            }
        } 
        else 
        {
            // Only exit apartment if door is triggered
            if (apartment101.is_door_triggered()) 
            {
                // When exiting apartment, spawn at the apartment 101 door position
                current_level = LEVEL_HALLWAY;
                
                // Position player at apartment 101 door position in hallway
                const float DOOR_X = 80.0f;  // X position of apartment 101 door
                player.set_position(point_at(DOOR_X, hallway.get_ground_level()));
                player.set_grounded(true);
                player.set_state(STATE_IDLE);
                
                // Reset camera
                camera_pos = point_at(DOOR_X - 400.0f, hallway.get_ground_level() - 300.0f);
                if (camera_pos.x < 0) camera_pos.x = 0;
                if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;
                
                nearby_interaction_prompt = "";
                write_line("Exited to hallway at apartment 101 door");
            }
        }
    }
    
    // --- Global Controls ---
    if (key_down(ESCAPE_KEY)) 
    {
        if (!in_hallway) 
        {
            // When pressing ESC in apartment, return to hallway at apartment 101 door
            current_level = LEVEL_HALLWAY;
            
            // Position player at apartment 101 door position in hallway
            const float DOOR_X = 80.0f;  // X position of apartment 101 door
            player.set_position(point_at(DOOR_X, hallway.get_ground_level()));
            player.set_grounded(true);
            player.set_state(STATE_IDLE);
            
            // Reset camera
            camera_pos = point_at(DOOR_X - 400.0f, hallway.get_ground_level() - 300.0f);
            if (camera_pos.x < 0) camera_pos.x = 0;
            if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;
            
            nearby_interaction_prompt = "";
            write_line("Returned to hallway at apartment 101 door (ESC)");
        } 
        else 
        {
            quit_game = true;  // Quit game from hallway
        }
    }
    
    // --- Debug Controls ---
    if (key_typed(R_KEY)) 
    {
        if (in_hallway) 
        {
            // Reset hallway position to lobby exit
            const float START_X = 1435.0f;
            player.set_position(point_at(START_X, hallway.get_ground_level()));
            player.set_grounded(true);
            player.set_state(STATE_IDLE);
            
            camera_pos = point_at(START_X - 400.0f, hallway.get_ground_level() - 300.0f);
            if (camera_pos.x < 0) camera_pos.x = 0;
            if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;
            
            // Reset hallway interactions
            hallway.setup_interactables();
        } 
        else 
        {
            // Reset apartment position to door position
            const float START_X = 100.0f;
            player.set_position(point_at(START_X, apartment101.get_ground_level()));
            player.set_grounded(true);
            player.set_state(STATE_IDLE);
            
            camera_pos = point_at(START_X - 400.0f, apartment101.get_ground_level() - 300.0f);
        }
        
        nearby_interaction_prompt = "";
    }
}