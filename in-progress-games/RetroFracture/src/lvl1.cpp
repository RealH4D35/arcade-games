#include "lvl1.h"
#include <iostream>

/**
 * Level1 constructor
 * Initializes apartment with default properties
 */
Level1::Level1() : 
    exit_door("exit_door", 100.0f, 610.0f, 40.0f, 60.0f, "EXIT TO HALLWAY"),
    door_triggered(false)
{
    // Set apartment geometry and colors
    ground_level = 700.0f;                              // Grey floor starts at y=700
    background_color = COLOR_BLACK;                     // Black background
    floor_color = rgba_color(128, 128, 128, 255);      // Grey floor
    
    write_line("Level 1 (Apartment 101) initialized");
}

/**
 * Update apartment level
 * @param player Player reference for potential updates
 */
void Level1::update(Player& player) 
{
    // Simple apartment - minimal updates needed
    // Future: Add NPCs, moving platforms, etc.
}

/**
 * Draw entire apartment level
 * @param player Player reference (for potential relative drawing)
 * @param camera_pos Camera position for scrolling
 */
void Level1::draw(const Player& player, const point_2d& camera_pos) const 
{
    // Draw apartment elements in order (back to front)
    draw_background(camera_pos);
    draw_floor(camera_pos);
    
    // Draw exit door
    exit_door.draw(camera_pos, 0);
}

/**
 * Draw apartment background
 * @param camera_pos Camera position (not used for solid color)
 */
void Level1::draw_background(const point_2d& camera_pos) const 
{
    clear_screen(background_color);  // Simple solid color background
}

/**
 * Draw apartment floor
 * @param camera_pos Camera position for vertical scrolling
 */
void Level1::draw_floor(const point_2d& camera_pos) const 
{
    // Calculate floor position relative to camera
    float screen_floor_y = (ground_level - camera_pos.y) - 30;
    
    // Draw main floor rectangle
    fill_rectangle(floor_color, 0, screen_floor_y, 800, 100);
    
    // Add floor details (stripes)
    for (int i = 0; i < 800; i += 40) 
    {
        draw_line(rgba_color(100, 100, 100, 255), 
                 i, screen_floor_y, 
                 i, screen_floor_y + 100);
    }
}

/**
 * Draw reference grid for visual alignment
 */
void Level1::draw_grid() const 
{
    // Draw vertical grid lines
    for (int x = 0; x < 800; x += 50) 
    {
        draw_line(rgba_color(230, 230, 230, 255), x, 0, x, 600);
    }
    
    // Draw horizontal grid lines
    for (int y = 0; y < 600; y += 50) 
    {
        draw_line(rgba_color(230, 230, 230, 255), 0, y, 800, y);
    }
}

/**
 * Check player boundaries in apartment
 * @param player Player to check boundaries for
 */
void Level1::check_boundaries(Player& player) 
{
    point_2d pos = player.get_position();
    
    // Left boundary (prevent leaving left side of screen)
    const float LEFT_BOUNDARY = 0.0f;
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
    
    // Ceiling collision
    const float CEILING_LEVEL = 50.0f;
    if (pos.y < CEILING_LEVEL) 
    {
        pos.y = CEILING_LEVEL;
        player.stop_vertical_movement();
    }
    
    // Apply corrected position
    player.set_position(pos);
    
    // Note: No right boundary - allows infinite running to the right
}

/**
 * Check for player interactions in apartment
 * Currently only checks exit door proximity
 * @param player Player to check interactions for
 */
void Level1::check_interactions(const Player& player) 
{
    door_triggered = exit_door.check_collision(player.get_position());
}