#include "interactable.h"
#include <iostream>

/**
 * Interactable constructor
 * @param id Unique identifier
 * @param x World X position
 * @param y World Y position
 * @param width Collision width
 * @param height Collision height
 * @param label Display label
 */
Interactable::Interactable(std::string id, float x, float y, float width, float height, std::string label)
    : id(id), x(x), y(y), width(width), height(height), 
      is_active(true), is_triggered(false), label(label) 
{
    // Initialization complete
}

/**
 * Draw interactable object
 * @param camera_pos Current camera position
 * @param background_y_offset Background vertical offset
 */
void Interactable::draw(point_2d camera_pos, float background_y_offset) const 
{
    if (!is_active) return;
    
    // Convert world coordinates to screen coordinates
    float screen_x = x - camera_pos.x;
    float screen_y = y - camera_pos.y + background_y_offset;
    
    // Draw interactable area (debug visualization)
    if (is_triggered) 
    {
        // Green rectangle for triggered state
        draw_rectangle(COLOR_GREEN, screen_x, screen_y, width, height);
    } 
    else 
    {
        // Yellow outline for available interaction
        draw_rectangle(COLOR_YELLOW, screen_x, screen_y, width, height);
        draw_rectangle(COLOR_BLACK, screen_x + 2, screen_y + 2, 
                      width - 4, height - 4);
    }
    
    // Draw label if available and not triggered
    if (!label.empty() && !is_triggered) 
    {
        // Calculate label position (centered above interactable)
        // Estimate text width - using fixed approximation since text_width might not be available
        float text_width_estimate = label.length() * 8.0f; // Approximate: 8 pixels per character
        float label_x = screen_x + (width - text_width_estimate) / 2;
        draw_text(label, COLOR_WHITE, label_x, screen_y - 20);
    }
    
    // Draw interaction indicator
    if (!is_triggered) 
    {
        float center_x = screen_x + width / 2 - 10;  // Center [E] text
        float center_y = screen_y + height / 2 - 10;
        draw_text("[E]", COLOR_CYAN, center_x, center_y);
    }
}

/**
 * Update interactable state
 * For animations, timers, or state changes
 */
void Interactable::update() 
{
    // Future: Add animation updates, timers, etc.
}

/**
 * Check collision with player
 * @param player_pos Player's world position
 * @return True if player is within interaction range
 */
bool Interactable::check_collision(point_2d player_pos) const 
{
    if (!is_active || is_triggered) return false;
    
    // Player collision box (based on sprite dimensions)
    const float PLAYER_HALF_WIDTH = 48.0f;
    const float PLAYER_HEIGHT = 96.0f;
    
    float player_left = player_pos.x - PLAYER_HALF_WIDTH;
    float player_right = player_pos.x + PLAYER_HALF_WIDTH;
    float player_top = player_pos.y - PLAYER_HEIGHT;
    float player_bottom = player_pos.y;
    
    // Interactable bounds
    float interact_right = x + width;
    float interact_bottom = y + height;
    
    // Check for bounding box overlap
    return !(player_left > interact_right || 
             player_right < x || 
             player_top > interact_bottom || 
             player_bottom < y);
}

/**
 * Trigger interaction with object
 * Marks object as triggered and performs any associated actions
 */
void Interactable::trigger() 
{
    if (!is_active || is_triggered) return;
    
    is_triggered = true;
    write_line("Interactable triggered: " + id + " (" + label + ")");
    
    // Future: Add sound effects, visual effects, or game state changes
}

/**
 * Reset interactable to initial state
 */
void Interactable::reset() 
{
    is_triggered = false;
    is_active = true;
}