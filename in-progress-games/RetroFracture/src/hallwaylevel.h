#ifndef HALLWAYLEVEL_H
#define HALLWAYLEVEL_H

#include "splashkit.h"
#include "player.h"
#include "interactable.h"
#include <vector>

/**
 * Hallway level class
 * Manages hallway background, interactables, and collision boundaries
 */
class HallwayLevel 
{
private:
    bitmap background;                    // Background bitmap
    float original_width, original_height; // Original image dimensions
    float scaled_width, scaled_height;    // Scaled dimensions for screen
    float scale_factor;                   // Scaling factor
    float y_offset;                       // Vertical offset for centering
    float ground_level;                   // Walkable ground level
    float left_boundary_offset;           // Offset from left edge to playable area
    float floor_offset;                   // Offset from bottom to floor level
    
    std::vector<Interactable> interactables; // List of interactable objects
    Interactable* nearby_interactable;       // Currently nearby interactable

public:
    HallwayLevel();                         // Constructor
    
    // Level lifecycle methods
    void load();                            // Load level resources
    void unload();                          // Unload level resources
    
    // Update and render
    void update(Player& player);            // Update level state
    void draw(const point_2d& camera_pos) const; // Draw level
    
    // Collision and interaction
    void check_boundaries(Player& player);  // Check player boundaries
    void check_interactions(const Player& player); // Check for interactions
    
    // Getters
    float get_ground_level() const { return ground_level; }
    float get_y_offset() const { return y_offset; }
    float get_left_boundary_offset() const { return left_boundary_offset; }
    float get_floor_offset() const { return floor_offset; }
    Interactable* get_nearby_interactable() const { return nearby_interactable; }
    const std::vector<Interactable>& get_interactables() const { return interactables; }
    const bitmap& get_background() const { return background; }
    
    // Setup methods
    void setup_interactables();             // Initialize interactable objects
    
    // Interaction triggers
    void trigger_interaction(const std::string& id); // Trigger specific interaction
};

#endif