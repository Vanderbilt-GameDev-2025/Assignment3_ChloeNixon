#ifndef CUSTOM_GRAVITY_H
#define CUSTOM_GRAVITY_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>

namespace godot {

class CustomGravity : public RigidBody3D {
    GDCLASS(CustomGravity, RigidBody3D);

protected:
    static void _bind_methods();

private:
    //determines how strong the force of gravity is
    float gravity_strength; 
    //the coordinates of the origin/center of gravity
    Vector3 gravity_center;
    //gravity mode: 0 for point, 1 for local, 2 for radial
    int gravity_mode = 0;

    //Variables of planet's physics at a given frame in physics process
    //Central planet
    CustomGravity* parent_planet;
    //collision shape of the central planet
    CollisionShape3D* collision_shape;
    //3d coordinates of the central planet
    Vector3 parent_position;
    //3d coordinates of the child planet
    Vector3 child_position;
    //direction the central planet is relative to the child planet
    Vector3 direction;
    //distance between the two planets
    float distance;
    //calculated gravitational force based on direction, gravity strength and distance
    Vector3 gravity_force;
    //velocity of child planet
    Vector3 velocity;

public:
    CustomGravity();
    ~CustomGravity();
    
    //Initializes variables
    void _ready() override;
    //Function overriding jolt forces process
    virtual void _integrate_forces(PhysicsDirectBodyState3D *state) override;

    void set_gravity_strength(float p_strength);
    float get_gravity_strength() const;
    void set_gravity_center(const Vector3 &p_center);
    Vector3 get_gravity_center() const;
    void set_gravity_mode(const int &p_mode);
    int get_gravity_mode() const;
};

} 

#endif // CUSTOM_GRAVITY_H