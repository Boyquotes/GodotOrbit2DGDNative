#ifndef __ORBITFOLLOW2D_H_
#define __ORBITFOLLOW2D_H_

#include <Godot.hpp>
#include <Path2D.hpp>
#include <Area2D.hpp>
#include <Curve2D.hpp>
#include <Sprite.hpp>
#include <PathFollow2D.hpp>
#include <PhysicsBody2D.hpp>

class OrbitPath2D : public godot::Path2D {
    GODOT_CLASS(OrbitPath2D, godot::Sprite
    )

private:
    // User Defined
    float semi_major_axis;
    float eccentricity;
    float argument_of_periapsis;
    float gravity;
    float gravity_distance_scale;
    float draw_resolution;
    godot::Color draw_color;

    // Memoizations
    godot::Vector2 _focus;
    float _semi_minor_axis;
    // float _cached_angular_velocity;
    // float _standard_gravitational_parameter;
    // PhysicsBody2D *_body;
    // PathFollow2D *_path_follow;
    // Area2D *_area;

public:
    static void _register_methods();

    OrbitPath2D();

    ~OrbitPath2D();

    // Godot functions
    void _init(); // our initializer called by Godot

    void _ready();

    //  void _physics_process(float delta);

    void _draw();

    // Important Functions
    void generate_path();

    void draw_ellipse();

    // Setters
    void set_semi_major_axis(const float value);

    void set_eccentricity(const float value);

    void set_argument_of_periapsis(const float value);

    void set_draw_resolution(const int value);

    void set_draw_color(const godot::Color value);

    //  void set_gravity(const float value);

    // Getters
    float get_semi_major_axis();

    float get_eccentricity();

    float get_argument_of_periapsis();

    int get_draw_resolution();

    godot::Color get_draw_color();

    // float get_gravity();

    // Memoized Getters
    float get_semi_minor_axis();

    // godot::Vector2 get_velocity();

    godot::Vector2 get_focus_point();

    // PhysicsBody2D *get_body();

    // PathFollow2D *get_path_follow();

    // Area2D *get_area();

    // float get_standard_gravitational_parameter();
};

#endif // __ORBITFOLLOW2D_H_
