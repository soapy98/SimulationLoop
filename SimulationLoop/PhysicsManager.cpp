#include "PhysicsManager.h"

physics_manager::physics_manager(vector<game_object*>& gameObjects) : m_game_objects_(gameObjects),m_gravity_(XMVectorSet(0, -9.81f, 0, 0))
{
}
void physics_manager::calculate_game_object_physics(const float dt) const
{
	for (const game_object* game_object : m_game_objects_)
	{
		if (game_object->get_rigid_body_component())
		{
			//Improved Euler
			if (game_object->get_rigid_body_component()->get_use_gravity())
			{
				rigid_body_component* rigid_body;
				rigid_body = game_object->get_rigid_body_component();

				// Check if rigid body is asleep and only awoke if a contact is made with another object.
				if (!rigid_body->get_is_awake())
				{
					return;
				}
				XMVECTOR dx{}, dx1{}, dx2{}, dx3{}, dx4{}, dv{}, dv1{}, dv2{}, dv3{}, dv4{};
				const auto mass = rigid_body->get_mass();
				const auto inverseMass = rigid_body->get_inverse_mass();
				//Gravity added to rigid body
				rigid_body->add_accumulated_force(m_gravity_ * mass);
				// Accumulated force and  linear acceleration accessed
				auto accumulatedForce = rigid_body->get_accumulated_force();
				auto accumulatedToque =  rigid_body->get_accumulated_torque();

				auto linearAcceleration = accumulatedForce / mass;

				linearAcceleration = linearAcceleration + (accumulatedForce * inverseMass);

				rigid_body->set_last_frame_acceleration(linearAcceleration);

				auto inverseInertiaTensorWorld =  rigid_body->get_inverse_inertia_tensor_world();
				//AngularAcceleration calculated
				auto angular_acc = XMVector3Transform(accumulatedToque, inverseInertiaTensorWorld);
				//Add drag to our velocity and angular velocity
				const auto drag = rigid_body->get_drag();
				const auto angularDrag = rigid_body->get_angular_drag();
				//Apply transformations on rigid body
				const auto velocity = rigid_body->get_velocity();
				const auto position =  rigid_body->get_position();
				const auto rotation =  rigid_body->get_rotation();
				const auto angularVelocity = rigid_body->get_angular_velocity();
				//Runge Kutta
				dx1 = velocity * dt;
				dv1 = (linearAcceleration * dt);

				dx2 = (velocity + dv1 / 2) * dt;
				dv2 = (linearAcceleration + linearAcceleration * dx1 / 2) * dt;

				dx3 = (velocity + dv2 / 2) * dt;
				dv3 = (linearAcceleration + linearAcceleration * dx2 / 2) * dt;

				dx4 = (velocity + dv3 / 2) * dt;
				dv4 = (linearAcceleration + linearAcceleration * dx3 / 2) * dt;
				//VelocityComponent and angular velocity and Position
				const auto newPosition = position + ((dx1 + dx2 * 2 + dx3 * 3 + dx4) / 6);

				auto newAngularVelocity = angularVelocity + (angular_acc * dt);
				XMVECTOR new_velocity = velocity + ((dv1 + dv2 * 2 + dv3 * 2 + dv4) / 6);
				
				//Drag added onto rigid body
				new_velocity = new_velocity * powf(drag, dt);
				newAngularVelocity = newAngularVelocity * powf(angularDrag, dt);
				auto angular_velocity_quaternion = XMQuaternionRotationRollPitchYawFromVector(newAngularVelocity * dt);
				const auto newRotation = XMQuaternionMultiply(rotation, angular_velocity_quaternion);
				//PositionComponent, velocity, rotation, angular velocity are updated along with new inverse inertia tensor in world space and any accumulated force cleared
				rigid_body->set_new_position(newPosition);
				rigid_body->set_new_velocity(new_velocity);
				rigid_body->set_rotation(newRotation);
				rigid_body->set_angular_velocity(newAngularVelocity);
				rigid_body->clear_accumulators();
			}
		}

	}
}

void physics_manager::update_game_object_physics() const 
{
	for (auto* gameObject : m_game_objects_)
	{
		if (gameObject->get_rigid_body_component()->get_use_gravity())
		{
			gameObject->get_rigid_body_component()->update_velocity();
			gameObject->get_rigid_body_component()->update_position();
		}

	}
}
