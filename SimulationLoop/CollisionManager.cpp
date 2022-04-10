#include "CollisionManager.h"

std::mutex collision_manager::collisionMutex;
collision_manager::collision_manager(vector<game_object*>& gameObjects, float friction, float restitution) :
	m_friction_(friction), m_restitution_(restitution), m_game_objects_(gameObjects),
	m_contact_manifold_(new contact_manifold())
{
	m_col_func_map_[make_pair(collider_component::collider_type::sphere, collider_component::collider_type::sphere)] = &collision_manager::sphere_on_sphere_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::peg, collider_component::collider_type::sphere)] = &collision_manager::cylinder_on_sphere_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::sphere, collider_component::collider_type::peg)] = &collision_manager::sphere_on_cylinder_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::plane, collider_component::collider_type::sphere)] = &collision_manager::plane_on_sphere_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::sphere, collider_component::collider_type::plane)] = &collision_manager::sphere_on_plane_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::aabb_cube, collider_component::collider_type::sphere)] = &collision_manager::aabb_on_sphere_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::sphere, collider_component::collider_type::aabb_cube)] = &collision_manager::sphere_on_aabb_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::obb_cube, collider_component::collider_type::sphere)] = &collision_manager::obb_on_sphere_detection;
	m_col_func_map_[make_pair(collider_component::collider_type::sphere, collider_component::collider_type::obb_cube)] = &collision_manager::sphere_on_obb_detection;
}
collision_manager::~collision_manager()
{
	if (m_contact_manifold_)
	{
		delete m_contact_manifold_;
		m_contact_manifold_ = nullptr;
	}
}

void collision_manager::dynamic_collision_detection() {
	std::lock_guard<std::mutex>lock(collisionMutex);
	m_contact_manifold_->Clear();

	for (unsigned int i = 0; i < m_game_objects_.size(); i++)
	{
		for (unsigned int j = i + 1; j < m_game_objects_.size(); j++)
		{
			const auto functionPointer = m_col_func_map_[make_pair(m_game_objects_.at(i)->get_collider_component()->get_collider(), m_game_objects_.at(j)->get_collider_component()->get_collider())];

			if (!functionPointer)
			{
				continue;
			}
			(this->*functionPointer)(m_game_objects_.at(i), m_game_objects_.at(j));
		}
	}
}


contact_manifold* collision_manager::get_contact_manifold_ref() const
{
	return m_contact_manifold_;
}

void collision_manager::sphere_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo) 
{
	const auto sphereOnePosition = gameObjectOne->get_rigid_body_component()->get_new_position();
	const auto sphereTwoPosition = gameObjectTwo->get_rigid_body_component()->get_new_position();
	const auto sphereOneScale = gameObjectOne->get_scale();
	const auto sphereTwoScale = gameObjectTwo->get_scale();

	const auto distance = sphereOnePosition - sphereTwoPosition;

	auto size = 0.0f;

	XMStoreFloat(&size, XMVector3Length(distance));

	const auto radiusSum = 1;

	//See if the squared distance is less than the squared radius sum
	if (size * size <= radiusSum * radiusSum)
	{
		//Generate contact (distance can be used as our midline)
		const auto normal = distance * (1.0f / size);

		const XMVECTOR contactPoint = sphereOnePosition + distance * 0.5f;

		ManifoldP contact = ManifoldP();
		contact.contactID[0] = gameObjectOne->get_rigid_body_component();
		contact.contactID[1] = gameObjectTwo->get_rigid_body_component();
		contact.contactNormal = normal;
		contact.contactPoint = contactPoint;
		contact.penetrationDepth = radiusSum - size;
		contact.friction = m_friction_;
		contact.restitution = m_restitution_;

		m_contact_manifold_->add(contact);
	}
}

void collision_manager::cylinder_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo)
{
	sphere_on_cylinder_detection(gameObjectTwo, gameObjectOne);
}

void collision_manager::sphere_on_cylinder_detection(game_object* gameObjectOne, game_object* gameObjectTwo)
{
	auto spherePosition = gameObjectOne->get_rigid_body_component()->get_new_position();
	auto cylinderPosition = gameObjectTwo->get_rigid_body_component()->get_position();

	auto sphereScale = gameObjectOne->get_scale();
	auto cylinderScale = gameObjectTwo->get_scale();
	//Lock peg z position to the spheres z position, this way we treat it as a 2D sphere collision
	XMVECTOR tempCylinderPosition = cylinderPosition;
	tempCylinderPosition = XMVectorSetZ(tempCylinderPosition, XMVectorGetZ(spherePosition));

	const auto distance = spherePosition - tempCylinderPosition;

	auto size = 0.0f;

	XMStoreFloat(&size, XMVector3Length(distance));

	const auto radiusSum = XMVectorGetX(sphereScale) + XMVectorGetX(cylinderScale);

	//See if the squared distance is less than the squared radius sum
	if (size * size <= radiusSum * radiusSum)
	{
		//Generate contact (distance can be used as our midline)
		const auto normal = distance * (1.0f / size);

		const XMVECTOR contactPoint = spherePosition + distance * 0.5f;

		ManifoldP contact = ManifoldP();
		contact.contactID[0] = gameObjectOne->get_rigid_body_component();
		contact.contactID[1] = nullptr;
		contact.contactNormal = normal;
		contact.contactPoint = contactPoint;
		contact.penetrationDepth = radiusSum - size;
		contact.friction = m_friction_;
		contact.restitution = m_restitution_;

		m_contact_manifold_->add(contact);
	}
}

//Just switch them around and redirect to the other function
void collision_manager::plane_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo) {
	sphere_on_plane_detection(gameObjectTwo, gameObjectOne);
}

void collision_manager::sphere_on_plane_detection(game_object* gameObjectOne, game_object* gameObjectTwo) {
	auto spherePosition = gameObjectOne->get_rigid_body_component()->get_new_position();
	auto sphereScale = gameObjectOne->get_scale();


	const plane_collider* planeCollider;
	planeCollider = dynamic_cast<plane_collider*>(gameObjectTwo->get_collider_component());
	auto planeNormal = planeCollider->get_normal();

	planeNormal = XMVectorSetY(planeNormal, -XMVectorGetY(planeNormal));

	auto sphereDistance = 0.0f;

	XMStoreFloat(&sphereDistance, XMVector3Dot(spherePosition, planeNormal));

	XMVectorScale(sphereScale, sphereDistance);
	//sphereDistance += sphereScale;
	sphereDistance += planeCollider->get_offset();

	if (sphereDistance <= 0.0f)
	{
		const XMVECTOR contactPoint = spherePosition - planeNormal * (XMVectorScale(sphereScale, sphereDistance));

		ManifoldP contact = ManifoldP();
		contact.contactID[0] = gameObjectOne->get_rigid_body_component();
		contact.contactID[1] = nullptr;
		contact.contactNormal = planeNormal;
		contact.contactPoint = contactPoint;
		contact.penetrationDepth = -sphereDistance;
		contact.friction = m_friction_;
		contact.restitution = m_restitution_;

		m_contact_manifold_->add(contact);
	}

}

void collision_manager::aabb_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo)
{
	sphere_on_aabb_detection(gameObjectTwo, gameObjectOne);
}
void collision_manager::sphere_on_aabb_detection(game_object* gameObjectOne, game_object* gameObjectTwo)
{
	//Need to get the closest point on our ABB to test against our sphere

	auto spherePosition = gameObjectOne->get_rigid_body_component()->get_new_position();

	auto cubePosition = gameObjectTwo->get_rigid_body_component()->get_position();
	auto sphereScale = gameObjectOne->get_scale();
	auto cubeScale = gameObjectTwo->get_scale();

	auto closestPoint = XMVECTOR();

	const auto cubePositionX = XMVectorGetX(cubePosition);
	const auto cubePositionY = XMVectorGetY(cubePosition);
	const auto cubePositionZ = XMVectorGetZ(cubePosition);

	const auto cubeScaleX = XMVectorGetX(cubeScale);
	const auto cubeScaleY = XMVectorGetY(cubeScale);
	const auto cubeScaleZ = XMVectorGetZ(cubeScale);

	//Need to get the closest point on our ABB to test against our sphere
	closestPoint = XMVectorSetX(closestPoint, max(cubePositionX - cubeScaleX, min(XMVectorGetX(spherePosition), cubePositionX + cubeScaleX)));
	closestPoint = XMVectorSetY(closestPoint, max(cubePositionY - cubeScaleY, min(XMVectorGetY(spherePosition), cubePositionY + cubeScaleY)));
	closestPoint = XMVectorSetZ(closestPoint, max(cubePositionZ - cubeScaleZ, min(XMVectorGetZ(spherePosition), cubePositionZ + cubeScaleZ)));

	const auto v = closestPoint - spherePosition;

	const auto vX = XMVectorGetX(v);
	const auto vY = XMVectorGetY(v);
	const auto vZ = XMVectorGetZ(v);

	const auto distanceSqrt = sqrt(vX * vX + vY * vY + vZ * vZ);

	if (distanceSqrt < XMVectorGetX(sphereScale))
	{
		ManifoldP contact = ManifoldP();
		contact.contactID[0] = gameObjectOne->get_rigid_body_component();
		contact.contactID[1] = nullptr;

		auto contactNormal = spherePosition - closestPoint;

		contactNormal = XMVector3Normalize(contactNormal);

		contact.contactNormal = contactNormal;
		contact.contactPoint = closestPoint;
		//This may be wrong
		contact.penetrationDepth = XMVectorGetX(sphereScale) - distanceSqrt;
		contact.friction = m_friction_;
		contact.restitution = m_restitution_;

		m_contact_manifold_->add(contact);
	}


}

void collision_manager::obb_on_sphere_detection(game_object* gameObjectOne, game_object* gameObjectTwo)
{
	sphere_on_obb_detection(gameObjectTwo, gameObjectOne);
}

void collision_manager::sphere_on_obb_detection(game_object* gameObjectOne, game_object* gameObjectTwo)
{
	//Projects the spherePosition within the local space of our OBB cube to get the closest point to it and then back to world coordinates to
	//test the distance between the sphere and the closest point on our OBB
	auto spherePosition =	gameObjectOne->get_rigid_body_component()->get_new_position();
	auto cubePosition =gameObjectTwo->get_rigid_body_component()->get_position();
	auto cubeRotation = gameObjectTwo->get_rigid_body_component()->get_rotation();
	auto sphereScale = gameObjectOne->get_scale();
	auto cubeScale = gameObjectTwo->get_scale();
	//Transform the sphere position to the local space of the cube
	XMMATRIX cubeTransform = XMMatrixRotationQuaternion(cubeRotation);

	cubeTransform = XMMatrixMultiply(cubeTransform, XMMatrixTranslationFromVector(cubePosition));

	auto determinant = XMMatrixDeterminant(cubeTransform);

	auto relativeSpherePosition = XMVector3Transform(spherePosition, XMMatrixInverse(&determinant, cubeTransform));

	auto closestPoint = XMVECTOR();
	auto distance = XMVectorGetX(relativeSpherePosition);

	const auto cubeScaleX = XMVectorGetX(cubeScale);
	const auto cubeScaleY = XMVectorGetY(cubeScale);
	const auto cubeScaleZ = XMVectorGetZ(cubeScale);

	if (distance > cubeScaleX)
	{
		distance = cubeScaleX;
	}

	if (distance < -cubeScaleX)
	{
		distance = -cubeScaleX;
	}

	closestPoint = XMVectorSetX(closestPoint, distance);

	distance = XMVectorGetY(relativeSpherePosition);
	if (distance > cubeScaleY)
	{
		distance = cubeScaleY;
	}

	if (distance < -cubeScaleY)
	{
		distance = -cubeScaleY;
	}

	closestPoint = XMVectorSetY(closestPoint, distance);

	distance = XMVectorGetZ(relativeSpherePosition);

	if (distance > cubeScaleZ)
	{
		distance = cubeScaleZ;
	}

	if (distance < -cubeScaleZ)
	{
		distance = -cubeScaleZ;
	}

	closestPoint = XMVectorSetZ(closestPoint, distance);

	const auto v = closestPoint - relativeSpherePosition;

	const auto vX = XMVectorGetX(v);
	const auto vY = XMVectorGetY(v);
	const auto vZ = XMVectorGetZ(v);

	const auto distanceSqrt = sqrt(vX * vX + vY * vY + vZ * vZ);

	if (distanceSqrt < XMVectorGetX(sphereScale))
	{
		auto closestPointWorld = XMVECTOR();

		closestPointWorld = XMVector3Transform(closestPoint, cubeTransform);

		ManifoldP contact = ManifoldP();
		contact.contactID[0] = gameObjectOne->get_rigid_body_component();
		contact.contactID[1] = nullptr;

		if (!gameObjectOne->get_rigid_body_component()->get_use_gravity())
		{
			return;
		}

		if (gameObjectTwo->get_rigid_body_component()->get_use_gravity())
		{
			contact.contactID[1] = gameObjectTwo->get_rigid_body_component();
		}

		contact.contactID[1] = nullptr;

		auto contactNormal = spherePosition - closestPointWorld;

		contactNormal = XMVector3Normalize(contactNormal);

		contact.contactNormal = contactNormal;
		contact.contactPoint = closestPointWorld;
		//This may be wrong
		contact.penetrationDepth = XMVectorGetX(sphereScale) - distanceSqrt;
		contact.friction = m_friction_;
		contact.restitution = m_restitution_;

		m_contact_manifold_->add(contact);
	}
}



