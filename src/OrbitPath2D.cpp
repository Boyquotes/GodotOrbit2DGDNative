#include "OrbitFollow2D.h"
#include "orbits.h"

static void OrbitPath2D::_register_methods();

OrbitPath2D::OrbitPath2D();
OrbitPath2D::~OrbitPath2D();

// Godot functions
void OrbitPath2D::_init() {}

void OrbitPath2D::_ready() {
    _path_follow = get_node("Follow");
    _body = get_node("Follow/Body");
    _area = get_node("Follow/Body/Gravity");
}

void OrbitPath2D::_physics_process(float delta) {
	path_follow.set_offset(follow.get_offset() + get_velocity() * delta);
}

void OrbitPath2D::_draw() {draw_ellipse(100);}

void OrbitPath2D::draw_ellipse(int nb_points) {
	// REF: https://www.mathopenref.com/coordcirclealgorithm.html
	float theta = 0.0;  // angle that will be increased each loop
	const float step = M_2_PI / nb_points;  // amount to add to theta each time (radians)
	PoolVector2Array points;

	int i = 0;
	while (theta < M_2_PI) {
		const real_t x = semi_major_axis*Math::cos(theta) + _focus.x;
		const real_t y = semi_minor_axis*Math::sin(theta) + _focus.y;
		// Rotate by argument of periapsis
		const real_t x_ = x*Math::cos(argument_of_periapsis) - y*Math::sin(argument_of_periapsis);
		const real_t y_ = x*Math::sin(argument_of_periapsis) + y*Math::cos(argument_of_periapsis);
		points.push_back(Vector2{x_, y_}-_focus);
		theta += step;
		i++;
	}
	for (int i = 0; i < nb_points-1; i++) {
		draw_line(points[i], points[i+1], color);
	}
	draw_line(points[nb_points-1], points[0], color);
}

// Important Functions
void OrbitPath2D::generate_path() {
    const float a = semi_major_axis;
    const float b = semi_minor_axis;
    curve.clear_points();
	curve.add_point(position+Vector2(b,0),Vector2(0,-a));
	curve.add_point(position+Vector2(0,a),Vector2(b,0));
	curve.add_point(position+Vector2(-b,0),Vector2(0,a));
	curve.add_point(position+Vector2(0,-a),Vector2(-b,0));
	curve.add_point(position+Vector2(b,0),Vector2(0,-a));
}

// Setters
void OrbitPath2D::set_semi_major_axis(const float value) {
    semi_major_axis = value;
    _semi_minor_axis = get_semi_minor_axis(eccentricity, semi_major_axis);
    _focus = get_focus_point(eccentricity, semi_major_axis, argument_of_periapsis);
    _cached_angular_velocity = 0.0;
    generate_path();
    update();
}
void OrbitPath2D::set_eccentricity(const float value) {
    eccentricity = value;
    _focus = get_focus_point(eccentricity, semi_major_axis, argument_of_periapsis);
    _cached_angular_velocity = 0.0;
    generate_path();
    update();
};
void OrbitPath2D::set_argument_of_periapsis(const float value) {
    argument_of_periapsis = value;
    _focus = get_focus_point(eccentricity, semi_major_axis, argument_of_periapsis);
    _cached_angular_velocity = 0.0;
    generate_path();
    update();
};
void OrbitPath2D::set_gravity(const float value) {
    gravity = value;
    _standard_gravitational_area = gravity / Math::pow(gravity_distance_scale, 2.0);
    _cached_angular_velocity = 0.0;
}
void OrbitPath2D::set_gravity_distance_scale(const float value) {
    gravity_distance_scale = value;
    _standard_gravitational_area = gravity / Math::pow(gravity_distance_scale, 2.0);
    _cached_angular_velocity = 0.0;
}

// Getters
float OrbitPath2D::get_semi_major_axis() {return semi_major_axis;}
float OrbitPath2D::get_eccentricity() {return eccentricity;}
float OrbitPath2D::get_argument_of_periapsis() {return argument_of_periapsis;}
float OrbitPath2D::get_gravity() {return gravity;}
float OrbitPath2D::get_gravity_distance_scale() {return gravity_distance_scale;}

// Memoized Getters
float OrbitPath2D::get_semi_minor_axis() {
  return _semi_minor_axis;
}
Vector2 OrbitPath2D::get_velocity() {
    // TODO: Handle circular obits with a cached velocity
    Vector2 v = get_heliocentric_velocity_from_eccentric_anomaly(
        get_eccentric_anomaly_from_position(position, focus_point),
        eccentricity,
        semi_major_axis,
        argument_of_periapsis,
        get_standard_gravitational_parameter()
    );
}
PhysicsBody2D* OrbitPath2D::get_body() {
    return _body;
}
PathFollow2D* OrbitPath2D::get_path_follow() {
    return _path_follow;
}
Area2D* OrbitPath2D::get_area() {
    return _area;
}
float OrbitPath2D::get_standard_gravitational_parameter() {
    return _standard_gravitational_area;
}
Vector2 OrbitPath2D::get_focus_point() {
    return _focus;
}
