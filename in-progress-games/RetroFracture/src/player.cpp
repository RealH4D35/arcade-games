#include "player.h"

/**
 * Player constructor
 * Initializes player properties and loads animations
 */
Player::Player() 
{
    // Initialize position (will be set by Game)
    position = point_at(0, 0);
    
    // Movement physics properties
    speed = 5.0f;
    jump_force = -12.0f;
    gravity = 1.0f;
    vertical_velocity = 0;
    
    // State flags
    is_grounded = true;
    facing_right = true;
    
    // Animation properties
    current_frame_index = 0;
    animation_timer = 0;
    current_state = STATE_IDLE;
    current_animation = nullptr;
    
    // Load all animations
    setup_animations();
    
    // Start with idle animation
    set_state(STATE_IDLE);
}

/**
 * Get player's collision bounding box
 * @return Rectangle representing player collision area
 */
rectangle Player::get_bounding_box() const 
{
    if (!current_animation) 
    {
        // Default bounding box if no animation loaded
        return rectangle_from(position.x - 48, position.y - 96, 96, 96);
    }
    
    // Calculate bounding box based on current animation frame
    float width = current_animation->frame_width;
    float height = current_animation->frame_height;
    
    // Position represents center bottom of sprite
    return rectangle_from(
        position.x - width / 2,   // Left edge
        position.y - height,      // Top edge
        width,                    // Width
        height                    // Height
    );
}

/**
 * Setup all player animations
 * Loads sprite sheets and configures animation properties
 */
void Player::setup_animations() 
{
    // Load each animation state
    load_animation("assets/sprites/striker/spr_StrikerIdle_strip.png", 
                   STATE_IDLE, 8, 96, 96, 10, true);
    
    load_animation("assets/sprites/striker/spr_StrikerRun_strip.png", 
                   STATE_RUN, 8, 96, 96, 6, true);
    
    load_animation("assets/sprites/striker/spr_StrikerJump_strip.png", 
                   STATE_JUMP, 12, 96, 96, 8, false);
    
    load_animation("assets/sprites/striker/spr_StrikerJump_strip.png", 
                   STATE_FALL, 12, 96, 96, 8, false);
    
    write_line("Player animations loaded successfully");
}

/**
 * Load specific animation from file
 * @param filename Path to sprite sheet
 * @param state Animation state to associate with
 * @param frame_count Number of frames in animation
 * @param frame_width Width of each frame
 * @param frame_height Height of each frame
 * @param speed Animation speed (frame delay)
 * @param loops Whether animation should loop
 */
void Player::load_animation(const std::string& filename, PlayerState state, 
                           int frame_count, int frame_width, int frame_height, 
                           int speed, bool loops) 
{
    Animation anim;
    
    // Generate unique bitmap name
    std::string bitmap_name = "anim_" + std::to_string(state);
    anim.anim_bitmap = load_bitmap(bitmap_name, filename);
    
    // Set animation properties
    anim.frame_count = frame_count;
    anim.frame_width = frame_width;
    anim.frame_height = frame_height;
    anim.animation_speed = speed;
    anim.loops = loops;
    
    // Set animation name for debugging
    switch(state) 
    {
        case STATE_IDLE: anim.name = "Idle"; break;
        case STATE_RUN: anim.name = "Run"; break;
        case STATE_JUMP: anim.name = "Jump"; break;
        case STATE_FALL: anim.name = "Fall"; break;
        default: anim.name = "Unknown";
    }
    
    // Store animation in map
    animations[state] = anim;
}

/**
 * Update player physics and animation
 * Called once per frame
 */
void Player::update() 
{
    // Apply gravity if not grounded
    if (!is_grounded) 
    {
        vertical_velocity += gravity;
        position.y += vertical_velocity;
        
        // Transition from jump to fall when starting to descend
        if (vertical_velocity > 0 && current_state == STATE_JUMP) 
        {
            set_state(STATE_FALL);
        }
    }
    
    // Update animation frame
    update_animation();
}

/**
 * Stop vertical movement immediately
 * Used for ceiling collisions
 */
void Player::stop_vertical_movement() 
{
    vertical_velocity = 0;
}

/**
 * Update animation frame based on timer
 * Advances to next frame when timer reaches animation speed
 */
void Player::update_animation() 
{
    if (!current_animation) return;
    
    animation_timer++;
    
    // Check if it's time to advance to next frame
    if (animation_timer >= current_animation->animation_speed) 
    {
        animation_timer = 0;
        current_frame_index++;
        
        // Handle end of animation
        if (current_frame_index >= current_animation->frame_count) 
        {
            if (current_animation->loops) 
            {
                current_frame_index = 0;  // Loop animation
            } 
            else 
            {
                current_frame_index = current_animation->frame_count - 1; // Hold last frame
                
                // Special handling for jump/fall animations
                if (current_state == STATE_JUMP && !is_grounded) 
                {
                    set_state(STATE_FALL);
                }
            }
        }
    }
}

/**
 * Change player animation state
 * @param new_state State to transition to
 */
void Player::set_state(PlayerState new_state) 
{
    // Don't change if already in this state
    if (current_state == new_state && current_animation != nullptr) return;
    
    current_state = new_state;
    
    // Find and set new animation
    if (animations.find(new_state) != animations.end()) 
    {
        current_animation = &animations[new_state];
        current_frame_index = 0;
        animation_timer = 0;
        
        // Special case: Fall animation starts at frame 6
        if (new_state == STATE_FALL) 
        {
            current_frame_index = 6;
        }
    }
}

/**
 * Draw player at world position (debug/legacy method)
 */
void Player::draw() 
{
    draw_at(position);
}

/**
 * Draw player at specific screen position
 * @param screen_position Position on screen to draw at
 */
void Player::draw_at(point_2d screen_position) 
{
    if (!current_animation) return;
    
    // Calculate source rectangle for current frame
    int frame_x = current_frame_index * current_animation->frame_width;
    int frame_y = 0;
    
    rectangle frame_rect = rectangle_from(frame_x, frame_y, 
                                         current_animation->frame_width, 
                                         current_animation->frame_height);
    
    // Calculate draw position (center horizontally, align feet to position)
    float draw_x = screen_position.x - current_animation->frame_width / 2;
    float draw_y = screen_position.y - current_animation->frame_height;
    
    // Setup drawing options
    drawing_options opts = option_part_bmp(frame_rect);
    
    // Flip sprite if facing left
    if (!facing_right) 
    {
        opts = option_scale_bmp(-1.0, 1.0, opts);
    }
    
    // Draw the sprite
    draw_bitmap(current_animation->anim_bitmap, draw_x, draw_y, opts);
}

/**
 * Move player left
 * Updates position and sets running animation if grounded
 */
void Player::move_left() 
{
    position.x -= speed;
    facing_right = false;
    
    // Only show running animation if on ground
    if (is_grounded && current_state != STATE_JUMP && current_state != STATE_FALL) 
    {
        set_state(STATE_RUN);
    }
}

/**
 * Move player right
 * Updates position and sets running animation if grounded
 */
void Player::move_right() 
{
    position.x += speed;
    facing_right = true;
    
    // Only show running animation if on ground
    if (is_grounded && current_state != STATE_JUMP && current_state != STATE_FALL) 
    {
        set_state(STATE_RUN);
    }
}

/**
 * Make player jump if grounded
 * Applies jump force and changes to jump state
 */
void Player::jump() 
{
    if (is_grounded) 
    {
        vertical_velocity = jump_force;
        is_grounded = false;
        set_state(STATE_JUMP);
    }
}

/**
 * Stop horizontal movement
 * Changes to idle state if on ground and running
 */
void Player::stop_moving() 
{
    if (is_grounded && current_state == STATE_RUN) 
    {
        set_state(STATE_IDLE);
    }
}