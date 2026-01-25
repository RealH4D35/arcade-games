#ifndef LVL1_H
#define LVL1_H

#include "splashkit.h"
#include "player.h"
#include "interactable.h"

/**
 * Level 1 - Apartment 101 class
 * Simple apartment level with infinite running to the right
 */
class Level1 
{
private:
    float ground_level;        // Walkable ground level
    color background_color;    // Background color
    color floor_color;         // Floor color
    
    Interactable exit_door;    // Exit door back to hallway
    bool door_triggered;       // Door interaction state

public:
    Level1();                  // Constructor
    
    // Level methods
    void update(Player& player);                          // Update level state
    void draw(const Player& player, const point_2d& camera_pos) const; // Draw level
    void check_boundaries(Player& player);                // Check player boundaries
    void check_interactions(const Player& player);        // Check for interactions
    
    // Getters
    float get_ground_level() const { return ground_level; }
    bool is_door_triggered() const { return door_triggered; }
    void reset_door() { door_triggered = false; }
    
    // Color setters
    void set_background_color(color bg) { background_color = bg; }
    void set_floor_color(color floor) { floor_color = floor; }
    
    // Drawing helper methods
    void draw_background(const point_2d& camera_pos) const;  // Draw background
    void draw_floor(const point_2d& camera_pos) const;       // Draw floor
    void draw_grid() const;                                  // Draw reference grid
};

#endif