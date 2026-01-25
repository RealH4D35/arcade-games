#include "hallwaylevel.h"
#include <iostream>

/**
 * HallwayLevel constructor
 * Initializes with default values in declaration order
 */
HallwayLevel::HallwayLevel() : 
    original_width(0), 
    original_height(0),
    scaled_width(0), 
    scaled_height(0),
    scale_factor(0), 
    y_offset(0), 
    ground_level(0),
    left_boundary_offset(0),  // New: offset from left edge of image to playable area
    floor_offset(0),          // New: offset from bottom of image to floor
    nearby_interactable(nullptr)
{
    // Initialization complete - resources loaded in load() method
}

/**
 * Load hallway level resources
 * Loads background image and sets up level geometry
 */
void HallwayLevel::load() 
{
    // Load hallway background image (UPDATED: Using Apartment Hallway.png)
    background = load_bitmap("hallway_bg", "assets/envs/Apartment Hallway.png");
    
    if (!background) 
    {
        write_line("ERROR: Could not load hallway background!");
        return;
    }
    
    // Calculate scaling and positioning
    original_width = bitmap_width(background);
    original_height = bitmap_height(background);
    
    write_line("Loaded Apartment Hallway.png - Dimensions: " + 
               std::to_string(original_width) + "x" + std::to_string(original_height));
    
    // IMPORTANT: Adjust these values based on your image!
    // ====================================================
    // left_boundary_offset: How many pixels from the LEFT edge of the image 
    //                       is the actual playable hallway area?
    // Example: If there's 100px of empty wall/space on the left, set to 100
    left_boundary_offset = -100.0f;  // ADJUST THIS!
    
    // floor_offset: How many pixels from the BOTTOM of the image 
    //               is the actual floor level?
    // Example: If the floor visually appears 50px from bottom, set to 50
    floor_offset = 150.0f;  // ADJUST THIS!
    // ====================================================
    
    // Scale to fit screen height
    const float TARGET_HEIGHT = 450.0f;
    scale_factor = TARGET_HEIGHT / original_height;
    scaled_height = original_height * scale_factor;
    scaled_width = original_width * scale_factor;
    
    // Also scale the offsets
    left_boundary_offset *= scale_factor;
    floor_offset *= scale_factor;
    
    write_line("Scaled dimensions: " + 
               std::to_string(scaled_width) + "x" + std::to_string(scaled_height) +
               " (Scale factor: " + std::to_string(scale_factor) + ")");
    write_line("Adjusted left boundary offset: " + std::to_string(left_boundary_offset));
    write_line("Adjusted floor offset: " + std::to_string(floor_offset));
    
    // Center vertically
    y_offset = (600.0f - scaled_height) / 2.0f;
    
    // Calculate ground level: bottom of image minus floor offset
    ground_level = scaled_height - floor_offset;
    
    write_line("Ground level set to: " + std::to_string(ground_level) + 
               " (Y offset: " + std::to_string(y_offset) + ")");
    
    // Setup interactable objects
    setup_interactables();
    
    write_line("Hallway level loaded successfully");
}

/**
 * Unload hallway level resources
 * Cleans up level data
 */
void HallwayLevel::unload() 
{
    nearby_interactable = nullptr;
    interactables.clear();
}

/**
 * Update hallway level state
 * @param player Reference to player for potential updates
 */
void HallwayLevel::update(Player& player) 
{
    // Update all interactables (animations, timers, etc.)
    for (auto& interactable : interactables) 
    {
        interactable.update();
    }
}

/**
 * Draw hallway level
 * @param camera_pos Current camera position for parallax/scrolling
 */
void HallwayLevel::draw(const point_2d& camera_pos) const 
{
    // Clear the screen to black before drawing hallway
    clear_screen(COLOR_BLACK);
    
    // Calculate draw position based on camera
    // Apply left_boundary_offset to shift the image so the playable area aligns with screen
    float draw_x = -camera_pos.x - left_boundary_offset;
    float draw_y = y_offset - camera_pos.y;
    
    // Draw scaled background
    drawing_options opts = option_scale_bmp(scale_factor, scale_factor);
    draw_bitmap(background, draw_x, draw_y, opts);
    
    // Draw all interactable objects
    for (const auto& interactable : interactables) 
    {
        interactable.draw(camera_pos, y_offset);
    }
    
    // Debug: Draw ground level line (remove in production)
    // draw_line(COLOR_RED, 0, y_offset + ground_level - camera_pos.y, 
    //           800, y_offset + ground_level - camera_pos.y);
    // Debug: Draw left boundary line (remove in production)
    // draw_line(COLOR_BLUE, left_boundary_offset, 0, 
    //           left_boundary_offset, 600);
}

/**
 * Check player boundaries in hallway
 * @param player Player to check boundaries for
 */
void HallwayLevel::check_boundaries(Player& player) 
{
    point_2d pos = player.get_position();
    const float SPRITE_HALF_WIDTH = 48.0f;
    
    // Left boundary: left_boundary_offset is where the actual playable area starts
    // So the left wall is at left_boundary_offset
    const float LEFT_BOUNDARY = left_boundary_offset + SPRITE_HALF_WIDTH;
    if (pos.x < LEFT_BOUNDARY) 
    {
        pos.x = LEFT_BOUNDARY;
    }
    
    // Ground collision
    if (pos.y > ground_level) 
    {
        pos.y = ground_level;
        if (!player.get_is_grounded() && player.get_state() == STATE_FALL) 
        {
            player.set_state(STATE_IDLE);
        }
        player.set_grounded(true);
    }
    
    // Apply corrected position
    player.set_position(pos);
}

/**
 * Check for player interactions with objects
 * @param player Player to check interactions for
 */
void HallwayLevel::check_interactions(const Player& player) 
{
    nearby_interactable = nullptr;
    
    // Check proximity to each interactable
    for (auto& interactable : interactables) 
    {
        if (interactable.check_collision(player.get_position())) 
        {
            nearby_interactable = &interactable;
            break;  // Only one nearby interactable at a time
        }
    }
}

/**
 * Setup hallway interactable objects
 * Creates doors and other interactive elements
 */
void HallwayLevel::setup_interactables() 
{
    interactables.clear();
    nearby_interactable = nullptr;
    
    // Add apartment doors along hallway
    // Format: ID, X, Y, Width, Height, Label
    
    interactables.push_back(Interactable("door1", 80.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 101"));
    interactables.push_back(Interactable("door2", 240.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 102"));
    interactables.push_back(Interactable("door3", 390.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 103"));
    interactables.push_back(Interactable("door5", 880.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 105"));
    interactables.push_back(Interactable("door6", 1050.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 106"));
    interactables.push_back(Interactable("door7", 1210.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 107"));
    interactables.push_back(Interactable("hexit", 1435.0f, ground_level - 25.0f, 40.0f, 60.0f, "Exit To Lobby"));
}

/**
 * Trigger interaction with specific object
 * @param id ID of interactable to trigger
 */
void HallwayLevel::trigger_interaction(const std::string& id) 
{
    for (auto& interactable : interactables) 
    {
        if (interactable.get_id() == id && !interactable.get_is_triggered()) 
        {
            interactable.trigger();
            break;
        }
    }
}