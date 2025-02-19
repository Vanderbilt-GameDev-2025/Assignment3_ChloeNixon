#include "CustomGravity.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
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
    gravity_center = Vector3(0,0,0);
}

// Physics processing that processes forces acting on the planet
// Gets the position of the parent planet, child planet, then applies force to the child based on their position
void CustomGravity::_integrate_forces(PhysicsDirectBodyState3D *state) {

    // planet parent & child postions
    parent_position = parent_planet->get_global_transform().origin;
    child_position = state->get_transform().origin;

    // if mode is on point, makes direction & distance relative to the gravity center
    if (gravity_mode == 0) {
        direction = (gravity_center - child_position).normalized();
        distance = child_position.distance_to(gravity_center);
    }

    // if gravity mode is in local, finds the collision shape of parent planet, and gravity is nearest point
    // to the child planet
    // radial zones is handled for when parent planet is a spherical collision box
    if (gravity_mode == 1 or gravity_mode == 2) {
        //shape of the collision model of the parent planet
        Shape3D *shape = *collision_shape->get_shape();
        if (shape) {
            Vector3 local_child_position = parent_planet->to_local(child_position);
            //coordinates of the point on the collisionshape closest to the child planet
            Vector3 point;

            //For when parent planet is a spherical object (current model)
            if (SphereShape3D *sphere = Object::cast_to<SphereShape3D>(shape)) {
                point = local_child_position.normalized() * sphere->get_radius();
            } 

            //For when parent planet is a rectangular box (model of child planet, but not parent)
            if (BoxShape3D *box = Object::cast_to<BoxShape3D>(shape)) {
                Vector3 extents = box->get_size() * 0.5;
                point = local_child_position.clamp(-extents, extents);
            } 

            Vector3 global_nearest_point = parent_planet->to_global(point);
            direction = (global_nearest_point - child_position).normalized();
            distance = child_position.distance_to(global_nearest_point);
        }
    }

    //calculates force based on gravity strength & distance between planets
    gravity_force = 10 * direction * gravity_strength;
     if (distance > 0.25) {
        gravity_force = gravity_force / distance; 
    }
    if (this != parent_planet) state->apply_central_force(gravity_force);

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