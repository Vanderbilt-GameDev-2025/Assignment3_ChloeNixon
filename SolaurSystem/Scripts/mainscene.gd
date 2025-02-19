extends Node3D

@onready var child_planet = $ChildPlanet

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if Input.is_action_just_pressed("quit"):
		get_tree().quit()
	elif Input.is_action_just_pressed("reset"):
		get_tree().reload_current_scene()


func _on_gravity_strength_slider_value_changed(value: float) -> void:
	child_planet.gravity_strength = value


func _on_center_slider_x_value_changed(value: float) -> void:
	child_planet.gravity_center[0] = int(value)

func _on_center_slider_y_value_changed(value: float) -> void:
	child_planet.gravity_center[1] = int(value)

func _on_center_slider_z_value_changed(value: float) -> void:
	child_planet.gravity_center[2] = int(value)


func _on_mode_slider_value_changed(value: float) -> void:
	child_planet.gravity_mode = int(value)
