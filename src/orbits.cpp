#include <cmath>
#include "kepler.h"
#include "orbits.h"

inline float get_semi_minor_axis(const float eccentricity, const float semi_major_axis) {
    return Math::sqrt(Math::pow(semi_major_axis, 2.0)-Math::pow(get_linear_eccentricity(eccentricity, semi_major_axis), 2.0));
}

inline float get_linear_eccentricity(const float eccentricity, const float semi_major_axis) {
    return eccentricity*semi_major_axis;
}

inline float get_geocentric_distance(
    const float standard_gravitational_parameter,
    const float rotational_period
) {
    return Math::pow(standard_gravitational_parameter*Math::pow(rotational_period, 2.0) / (4.0 * Math::pow(M_PI, 2.0)), 1.0/3.0);
}

inline Vector2 get_focus_point(const float eccentricity, const float semi_major_axis, const float argument_of_periapsis) {
    float le = linear_eccentricity(eccentricity, semi_major_axis);
    return Vector2{
        le*Math::sin(argument_of_periapsis),
        le*Math::cos(argument_of_periapsis)
    };
}

inline float get_orbital_period(const float semi_major_axis, const float standard_gravitational_parameter) {
    return 2.0 * M_PI * (Math::sqrt(Math::pow(semi_major_axis, 3.0) / standard_gravitational_parameter);
}

inline float get_mean_angular_motion(const float semi_major_axis, const float standard_gravitational_parameter) {
    return 2.0 * M_PI / get_orbital_period(semi_major_axis, standard_gravitational_parameter);
}

inline float get_mean_anomaly(const float time, const float semi_major_axis, const float standard_gravitational_parameter) {
    return get_mean_angular_motion(semi_major_axis, standard_gravitational_parameter) * time;
}

inline float get_eccentric_anomaly_from_mean_anomaly(const float mean_anomaly, const float eccentricity, const float argument_of_periapsis) {
    return ecc_anomaly(eccentricity, mean_anomaly) + argument_of_periapsis;
}

inline float get_eccentric_anomaly_from_position(
    const Vector2 position,
    const Vector2 focus_point
) {
    return focus_point.angle_to(position);
}

inline float get_eccentric_anomaly(const float time, const float eccentricity, const float semi_major_axis, const float argument_of_periapsis, const float standard_gravitational_parameter) {
    float mean_anomaly = get_mean_anomaly(time, semi_major_axis, standard_gravitational_parameter);
    return get_eccentric_anomaly_from_mean_anomaly(mean_anomaly, eccentricity, argument_of_periapsis);
}

inline float get_true_anomaly_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity) {
    const float E = eccentric_anomaly;
    return 2.0 * Math::atan2(Math::sqrt(1.0 + eccentricity) * Math::sin(E / 2.0), Math::sqrt(1.0 - eccentricity) * Math::cos(E / 2.0))
}

inline float get_true_anomaly_from_time(const float time, const float eccentricity, const float semi_major_axis, const float argument_of_periapsis, const float standard_gravitational_parameter) {
    float E = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis, standard_gravitational_parameter)
    return get_true_anomaly_from_eccentric(E, eccentricity);
}

inline float get_heliocentric_distance_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity, const float semi_major_axis) {
    return semi_major_axis * (1.0 - eccentricity*Math::cos(eccentric_anomaly));
}

inline float get_heliocentric_distance_from_time(const float time, const float eccentricity, const float semi_major_axis, const float argument_of_periapsis, const float standard_gravitational_parameter) {
    float eccentric_anomaly = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis, standard_gravitational_parameter);
    return get_heliocentric_distance_from_eccentric_anomaly(eccentric_anomaly, eccentricity, semi_major_axis)
}

Vector2 get_heliocentric_velocity_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity, const float semi_major_axis, const float argument_of_periapsis, const float standard_gravitational_parameter) {
    float heliocentric_distance = get_heliocentric_distance_from_eccentric_anomaly(eccentric_anomaly, eccentricity, semi_major_axis, argument_of_periapsis, standard_gravitational_parameter);
    float scale = Math::sqrt(standard_gravitational_parameter*semi_major_axis) / heliocentric_distance;
    return Vector2(
        scale*-Math::sin(eccentric_anomaly),
        scale*Math::sqrt(1.0-Math::pow(eccentricity, 2.0))*Math::cos(eccentric_anomaly)
    );
}

inline Vector2 get_heliocentric_velocity_from_time(const float time, const float eccentricity, const float semi_major_axis, const float argument_of_periapsis, const float standard_gravitational_parameter) {
    float eccentric_anomaly = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis, standard_gravitational_parameter);
    return get_heliocentric_velocity_from_eccentric_anomaly(eccentric_anomaly, eccentricity, semi_major_axis, argument_of_periapsis, standard_gravitational_parameter);
}

PositionVelocity2D get_heliocentric_position_velocity_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity, const float semi_major_axis, const float argument_of_periapsis, const float standard_gravitational_parameter) {
    float heliocentric_distance = get_heliocentric_distance_from_eccentric_anomaly(eccentric_anomaly, eccentricity, semi_major_axis, argument_of_periapsis, standard_gravitational_parameter);
    float scale = Math::sqrt(standard_gravitational_parameter*semi_major_axis) / heliocentric_distance;
    Vector2 v = Vector2(
        scale*-Math::sin(eccentric_anomaly),
        scale*Math::sqrt(1.0-Math::pow(eccentricity, 2.0))*Math::cos(eccentric_anomaly)
    );
    float distance = semi_major_axis * (1.0 - eccentricity*Math::cos(eccentric_anomaly));
    float true_anomaly = get_true_anomaly_from_eccentric(eccentric_anomaly);
    Vector2 p = Vector2(
        distance*Math::cos(true_anomaly);
        distance*Math::sin(true_anomaly);
    );
    return PositionVelocity2D(p, v);
}

inline PositionVelocity2D get_heliocentric_position_velocity_from_time(const float time, const float eccentricity, const float semi_major_axis, const float argument_of_periapsis, const float standard_gravitational_parameter) {
    float eccentric_anomaly = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis, standard_gravitational_parameter);
    return get_heliocentric_position_velocity_from_eccentric_anomaly(eccentric_anomaly, eccentricity,  semi_major_axis, argument_of_periapsis, standard_gravitational_parameter);
}

inline bool is_circle(float eccentricity) {
    return eccentricity == 0.0;
}
inline bool is_ellipse(float eccentricity) {
    return eccentricity > 0.0 && eccentricity < 1.0;
}
inline bool is_parabola(float eccentricity) {
    return eccentricity == 1.0;
}
inline bool is_hyperbola(float eccentricity) {
    return eccentricity > 1.0;
}
