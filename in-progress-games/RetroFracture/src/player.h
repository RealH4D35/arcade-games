#ifndef PLAYER_H
#define PLAYER_H

#include "splashkit.h"
#include <map>
#include <string>

/**
 * Player state enumeration
 * Defines possible animation states for the player character
 */
enum PlayerState 
{
    STATE_IDLE,     // Standing still
    STATE_RUN,      // Running left/right
    STATE_JUMP,     // Jumping upward
    STATE_FALL      // Falling downward
};

/**
 * Animation data structure
 * Contains all data needed to render a player animation
 */
struct Animation 
{
    bitmap anim_bitmap;     // Sprite sheet bitmap
    int frame_count;        // Number of frames in animation
    int frame_width;        // Width of each frame
    int frame_height;       // Height of each frame
    int animation_speed;    // Frame delay (lower = faster)
    bool loops;             // Whether animation loops
    std::string name;       // Animation name for debugging
};

/**
 * Player character class
 * Manages player movement, physics, animations, and rendering
 */
class Player 
{
private:
    point_2d position;                      // World position (center bottom)
    float speed;                            // Horizontal movement speed
    float jump_force;                       // Initial jump velocity
    float gravity;                          // Gravity force
    float vertical_velocity;                // Current vertical speed
    bool is_grounded;                       // Whether player is on ground
    bool facing_right;                      // Facing direction
    
    PlayerState current_state;              // Current animation state
    int current_frame_index;                // Current animation frame
    int animation_timer;                    // Animation frame timer
    std::map<PlayerState, Animation> animations; // All loaded animations
    Animation* current_animation;           // Current active animation

public:
    Player();                               // Constructor
    
    // Core methods
    void update();                          // Update physics and animation
    void draw();                            // Draw at world position (debug)
    void draw_at(point_2d screen_position); // Draw at specific screen position
    
    // State management
    void set_state(PlayerState new_state);  // Change animation state
    PlayerState get_state() const { return current_state; }
    
    // Movement methods
    void move_left();                       // Move left
    void move_right();                      // Move right
    void jump();                            // Jump if grounded
    void stop_moving();                     // Stop horizontal movement
    void stop_vertical_movement();          // Stop vertical movement
    
    // Getters and setters
    point_2d get_position() const { return position; }
    void set_position(point_2d new_pos) { position = new_pos; }
    bool get_is_grounded() const { return is_grounded; }
    void set_grounded(bool grounded) { is_grounded = grounded; }
    bool get_facing_right() const { return facing_right; }
    
    // Collision detection
    rectangle get_bounding_box() const;     // Get collision rectangle
    
    // Animation info
    int get_current_frame_index() const { return current_frame_index; }
    std::string get_current_animation_name() const { 
        return current_animation ? current_animation->name : "None"; 
    }
    
private:
    // Animation management
    void setup_animations();                                    // Initialize animations
    void load_animation(const std::string& filename,           // Load specific animation
                       PlayerState state, 
                       int frame_count, int frame_width, int frame_height, 
                       int speed, bool loops);
    void update_animation();                                   // Advance animation frames
};

#endif