#include "ResponseManager.h"

std::mutex response_manager::_responseMutex;
//Implmented with use of of Ed Milligton: Game Physics Engine Development
response_manager::response_manager(contact_manifold* contactManifold, const int positionIterations, const int velocityIterations, const float positionEpsilon, const float velocityEpsilon) : m_position_iterations_done_(0), m_position_iterations_(positionIterations), m_velocity_iterations_count_(0), m_velocity_count_(velocityIterations), m_pos_epsilon_(positionEpsilon), m_vel_epsilon_(velocityEpsilon), m_contact_manifold_(contactManifold)
{
}

void response_manager::contact_resolve(const float dt)
{
	std::lock_guard<std::mutex> lock(_responseMutex);
	m_position_iterations_done_ = 0;
	m_velocity_iterations_count_ = 0;

	//Return if no contacts then 
	if (!m_contact_manifold_->GetNumberOfPoints())
	{
		return;
	}

	//Prepare Contacts
	ready_contacts(dt);

	change_pos(dt);

	change_vel(dt);
}


void response_manager::change_vel(const float dt)
{
	//std::lock_guard<std::mutex> lock(_responseMutex);
	//Now need to update velocities
	XMVECTOR velocityChange[2]{};
	XMVECTOR angularVelocityChange[2];
	auto deltaVelocity = XMVECTOR();

	while (m_velocity_iterations_count_ < m_velocity_count_)
	{
		auto max = m_vel_epsilon_;
		auto index = m_contact_manifold_->GetNumberOfPoints();

		for (unsigned i = 0; i < m_contact_manifold_->GetNumberOfPoints(); i++)
		{
			const auto& point = m_contact_manifold_->GetPoint(i);

			if (point.desiredDeltaVelocity > max)
			{
				max = point.desiredDeltaVelocity;
				index = i;
			}
		}

		if (index == m_contact_manifold_->GetNumberOfPoints())
		{
			break;
		}

		auto& point = m_contact_manifold_->GetPoint(index);

		point.awake_state(); //Get awake state

		point.apply_velocity_change(velocityChange, angularVelocityChange);

		//Contacts updates with new velocity changes
		auto i = 0;
		while ( i < m_contact_manifold_->GetNumberOfPoints())
		{
			auto& localPoint = m_contact_manifold_->GetPoint(i);
			auto j = 0;
			while ( j < 2&& localPoint.contactID[j])
			{
				for (unsigned k = 0; k < 2; k++)
				{
					if (localPoint.contactID[j] == point.contactID[k])
					{
						deltaVelocity = XMVectorAdd(velocityChange[k], XMVector3Cross(angularVelocityChange[k], localPoint.relativeContactPosition[j]));

						const auto contactToWorldTranspose = XMMatrixTranspose(localPoint.contactToWorld);

						const auto contactDeltaVelocity = XMVector3Transform(deltaVelocity, contactToWorldTranspose);
						localPoint.contactVelocity += XMVectorScale(contactDeltaVelocity, j ? -1 : 1);
						localPoint.calculate_desired_delta_velocity(dt);
					}
				}
				j++;
			}
			i++;
		}
		m_velocity_iterations_count_++;
	}
}

void response_manager::change_pos(const float dt)
{
	//std::lock_guard<std::mutex> lock(_responseMutex);
	//PositionComponent chnage and resolving of penetrations
	unsigned int i = 0;
	unsigned int index = 0;

	XMVECTOR linearChange[2];
	XMVECTOR angularChange[2];

	auto deltaPosition = XMVECTOR();
	auto maxPenetration = 0.0f;

	m_position_iterations_done_ = 0;

	while (m_position_iterations_done_ < m_position_iterations_)
	{
		//Largest peneration to be found
		maxPenetration = m_pos_epsilon_; // Small value set so any small penetrations cna be ignored

		for (i = 0; i < m_contact_manifold_->GetNumberOfPoints(); i++)
		{
			auto& point = m_contact_manifold_->GetPoint(i);

			if (point.penetrationDepth > maxPenetration)
			{
				maxPenetration = point.penetrationDepth;
				index = i;
			}
		}

		if (index == m_contact_manifold_->GetNumberOfPoints())
		{
			break;
		}

		auto& point = m_contact_manifold_->GetPoint(index);

		point.awake_state(); //Match awake state

		point.resolve_penetration(linearChange, angularChange, maxPenetration);

		//Update contacts with new penetrations so the same penetration isn't resolved again
		//And also so other contacts don't have the wrong penetrations
		auto i = 0;
		while (i < m_contact_manifold_->GetNumberOfPoints())
		{
			auto& otherPoint = m_contact_manifold_->GetPoint(i);
			auto b = 0;
			while ( b < 2&&otherPoint.contactID[b]) 
			{
				for (unsigned int d = 0; d < 2; d++)
				{
					if (otherPoint.contactID[b] == point.contactID[d])
					{
						deltaPosition = XMVectorAdd(linearChange[d], XMVector3Cross(angularChange[d], otherPoint.relativeContactPosition[b]));
						XMStoreFloat(&otherPoint.penetrationDepth, XMVectorAdd(XMLoadFloat(&otherPoint.penetrationDepth), XMVectorScale(XMVector3Dot(deltaPosition, otherPoint.contactNormal), (b ? 1 : -1))));
					}
				}
				b++;
			}
			i++;
		}
		m_position_iterations_done_++;
	}
}
void response_manager::ready_contacts(const float dt) const
{
	//std::lock_guard<std::mutex> lock(_responseMutex);
	for (auto collision = 0; collision < m_contact_manifold_->GetNumberOfPoints(); ++collision)
	{
		auto& point = m_contact_manifold_->GetPoint(collision);

		point.internals_calcs(dt);
	}
}
