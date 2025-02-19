#include "CustomGravity.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>


using namespace godot;

void CustomGravity::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_gravity_mode", "mode"), &CustomGravity::set_gravity_mode);
    ClassDB::bind_method(D_METHOD("get_gravity_mode"), &CustomGravity::get_gravity_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gravity_mode", PROPERTY_HINT_ENUM, "Point,Local,Radial"), "set_gravity_mode", "get_gravity_mode");

    ClassDB::bind_method(D_METHOD("set_gravity_strength", "strength"), &CustomGravity::set_gravity_strength);
    ClassDB::bind_method(D_METHOD("get_gravity_strength"), &CustomGravity::get_gravity_strength);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gravity_strength"), "set_gravity_strength", "get_gravity_strength");

    ClassDB::bind_method(D_METHOD("set_gravity_center", "center"), &CustomGravity::set_gravity_center);
    ClassDB::bind_method(D_METHOD("get_gravity_center"), &CustomGravity::get_gravity_center);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "gravity_center"), "set_gravity_center", "get_gravity_center");
}

CustomGravity::CustomGravity() {

}

CustomGravity::~CustomGravity() {

}

void CustomGravity::_ready() {
    parent_planet = Object::cast_to<CustomGravity>(get_node_or_null("/root/MainScene/ParentPlanet"));
    collision_shape = Object::cast_to<CollisionShape3D>(get_node_or_null("/root/MainScene/ParentPlanet/Parent_CollisionShape"));
    //collision_shape = parent_planet->get_node<CollisionShape3D>("Parent_CollisionShape");
    UtilityFunctions::print("Collision: ", collision_shape);

    gravity_center = Vector3(0,0,0);
}

// Physics processing that processes forces acting on the planet
// Gets the position of the parent planet, child planet, then applies force to the child based on their position
void CustomGravity::_integrate_forces(PhysicsDirectBodyState3D *state) {

    //dont apply forces to parent planet
    if (this == parent_planet) {
        return;
    }  

    //planet parent & child postions
    parent_position = parent_planet->get_global_transform().origin;
    child_position = state->get_transform().origin;

    // if mode is on point, makes direction & distance relative to the gravity center
    if (gravity_mode == 0) {
        direction = (gravity_center - child_position).normalized();
        distance = child_position.distance_to(gravity_center);
    }

    // // if gravity mode is in local, finds the collision shape of the thing
    // if (gravity_mode == 1) {
    //     if (collision_shape) {
    //         Transform3D parent_transform = parent_planet->get_global_transform();
    //         Vector3 nearest_point = parent_transform.xform_inv(child_position); 
    //         nearest_point = collision_shape->get_shape()->project_local_ray_normal(nearest_point.normalized());
    //         nearest_point = parent_transform.xform(nearest_point); 

    //         direction = (nearest_point - child_position).normalized();
    //         distance = child_position.distance_to(nearest_point);
    //     } else {
    //         UtilityFunctions::print("No CollisionShape3D found on ParentPlanet!");
    //         return;
    //     }
    // }

    //calculates force based on gravity strength & distance between planets
    gravity_force = 10 * direction * gravity_strength;
     if (distance > 0.25) {
        gravity_force = gravity_force / distance; 
    }
    if (this != parent_planet) state->apply_central_force(gravity_force);

    //velocity = state->get_linear_velocity();
    //UtilityFunctions::print("Velocity: ", velocity);
}

void CustomGravity::set_gravity_mode(const int &p_mode) {
    gravity_mode = p_mode;
}
int CustomGravity::get_gravity_mode() const {
    return gravity_mode;
}

float CustomGravity::get_gravity_strength() const {
    return gravity_strength;
}
void CustomGravity::set_gravity_strength(float p_strength) {
    gravity_strength = p_strength;
}

void CustomGravity::set_gravity_center(const Vector3 &p_center) {
    gravity_center = p_center; 
}
Vector3 CustomGravity::get_gravity_center() const {
    return gravity_center;
}