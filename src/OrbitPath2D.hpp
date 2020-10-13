#ifndef __ORBITFOLLOW2D_H_
#define __ORBITFOLLOW2D_H_

#include <Godot.hpp>
#include <Path2D.hpp>
#include <PathFollow2D.hpp>
#include <PhysicsBody2D.hpp>

namespace godot {

class OrbitPath2D : public Path2D {
    GODOT_CLASS(OrbitPath2D, Sprite)

private:
    // User Defined
    float semi_major_axis;
    float eccentricity;
    float argument_of_periapsis;
    float gravity;
    float gravity_distance_scale;

    // Memoizations
    Vector2 _focus;
    float _semi_minor_axis;
    float _cached_angular_velocity;
    PhysicsBody2D* _body;
    PathFollow2D* _path_follow;
    Area2D* _area;
    float _standard_gravitational_parameter;

public:
    static void _register_methods();

    OrbitPath2D();
    ~OrbitPath2D();

    // Godot functions
    void _init(); // our initializer called by Godot

    void _ready();

    void _physics_process(float delta);

    void _draw();

    // Important Functions
    void generate_path();
    void draw_ellipse();

    // Setters
    void set_semi_major_axis(const float value);
    void set_eccentricity(const float value);
    void set_argument_of_periapsis(const float value);
    void set_gravity(const float value);
    void set_gravity_distance_scale(const float value);

    // Getters
    float get_semi_major_axis();
    float get_eccentricity();
    float get_argument_of_periapsis();
    float get_gravity();
    float get_gravity_distance_scale();

    // Memoized Getters
    float get_semi_minor_axis();
    Vector2 get_velocity();
    Vector2 get_focus_point();
    PhysicsBody2D* get_body();
    PathFollow2D* get_path_follow();
    Area2D* get_area();
    float get_standard_gravitational_parameter();
};

}


#endif // __ORBITFOLLOW2D_H_
