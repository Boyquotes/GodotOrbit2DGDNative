#include "OrbitPath2D.hpp"
#include "orbits.hpp"

// TODO
//static void OrbitPath2D::_register_methods();
//OrbitPath2D::OrbitPath2D();
//OrbitPath2D::~OrbitPath2D();

// Godot functions
void OrbitPath2D::_init() {}
void OrbitPath2D::_ready() {}

//void OrbitPath2D::_physics_process(float delta) {
//	_path_follow->set_offset(_path_follow->get_offset() + get_velocity() * delta);
//}

void OrbitPath2D::_draw() {draw_ellipse();}

void OrbitPath2D::draw_ellipse() {
	// REF: https://www.mathopenref.com/coordcirclealgorithm.html
	const int nb_points = get_draw_resolution();
	float theta = 0.0;  // angle that will be increased each loop
	const float step = M_2_PI / nb_points;  // amount to add to theta each time (radians)
    auto centroid = orbits::get_centroid_from_focus_point(eccentricity, semi_major_axis, argument_of_periapsis, get_focus_point());
	godot::PoolVector2Array points;

	int i = 0;
	while (theta < M_2_PI) {
		const real_t x = semi_major_axis*godot::Math::cos(theta) + centroid.x;
		const real_t y = get_semi_minor_axis()*godot::Math::sin(theta) + centroid.y;
		// Rotate by argument of periapsis
		const real_t x_ = x*godot::Math::cos(argument_of_periapsis) - y*godot::Math::sin(argument_of_periapsis);
		const real_t y_ = x*godot::Math::sin(argument_of_periapsis) + y*godot::Math::cos(argument_of_periapsis);
		points.push_back(godot::Vector2{x_, y_}-centroid);
		theta += step;
		i++;
	}
	for (int i = 0; i < nb_points-1; i++) {
		draw_line(points[i], points[i+1], draw_color);
	}
	draw_line(points[nb_points-1], points[0], draw_color);
}

// Important Functions
void OrbitPath2D::generate_path() {
    const float a = semi_major_axis;
    const float b = get_semi_minor_axis();
    auto curve = get_curve();
    auto centroid = orbits::get_centroid_from_focus_point(eccentricity, semi_major_axis, argument_of_periapsis, get_focus_point());
    curve->clear_points();
	curve->add_point(centroid+godot::Vector2(b,0),godot::Vector2(0,-a));
	curve->add_point(centroid+godot::Vector2(0,a),godot::Vector2(b,0));
	curve->add_point(centroid+godot::Vector2(-b,0),godot::Vector2(0,a));
	curve->add_point(centroid+godot::Vector2(0,-a),godot::Vector2(-b,0));
	curve->add_point(centroid+godot::Vector2(b,0),godot::Vector2(0,-a));
}

// Setters
void OrbitPath2D::set_semi_major_axis(const float value) {
    semi_major_axis = value;
    _semi_minor_axis = orbits::get_semi_minor_axis(eccentricity, semi_major_axis);
    // _cached_angular_velocity = 0.0;
    generate_path();
    update();
}
void OrbitPath2D::set_eccentricity(const float value) {
    eccentricity = value;
    // _cached_angular_velocity = 0.0;
    generate_path();
    update();
}
void OrbitPath2D::set_argument_of_periapsis(const float value) {
    argument_of_periapsis = value;
    // _cached_angular_velocity = 0.0;
    generate_path();
    update();
}
//void OrbitPath2D::set_gravity(const float value) {
//    gravity = value;
//    _standard_gravitational_area = gravity / godot::Math::pow(gravity_distance_scale, 2.0);
//    _cached_angular_velocity = 0.0;
//}
void OrbitPath2D::set_draw_resolution(const int value) {
    draw_resolution = value;
    generate_path();
    update();
}
void OrbitPath2D::set_draw_color(const godot::Color value) {
    draw_color = value;
    generate_path();
    update();
}

// Getters
float OrbitPath2D::get_semi_major_axis() {return semi_major_axis;}
float OrbitPath2D::get_eccentricity() {return eccentricity;}
float OrbitPath2D::get_argument_of_periapsis() {return argument_of_periapsis;}
//float OrbitPath2D::get_gravity() {return gravity;}
int OrbitPath2D::get_draw_resolution() {return draw_resolution;}

// Memoized Getters
float OrbitPath2D::get_semi_minor_axis() {
  return _semi_minor_axis;
}
//godot::Vector2 OrbitPath2D::get_velocity() {
//    // TODO: Handle circular obits with a cached velocity
//    godot::Vector2 v = get_heliocentric_velocity_from_eccentric_anomaly(
//        get_eccentric_anomaly_from_position(position, focus_point),
//        eccentricity,
//        semi_major_axis,
//        argument_of_periapsis,
//        get_standard_gravitational_parameter()
//    );
//}
//PhysicsBody2D* OrbitPath2D::get_body() {
//    return _body;
//}
//PathFollow2D* OrbitPath2D::get_path_follow() {
//    return _path_follow;
//}
//Area2D* OrbitPath2D::get_area() {
//    return _area;
//}
//float OrbitPath2D::get_standard_gravitational_parameter() {
//    return _standard_gravitational_area;
//}
godot::Vector2 OrbitPath2D::get_focus_point() {
    return get_position();
}
godot::Color OrbitPath2D::get_draw_color() {
    return draw_color;
}