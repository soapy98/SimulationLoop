#include "Renderer.h"
#include <iostream>
#include "Game.h"
#include "Timing.h"
#include "CalculatePhysicsJob.h"
#include "ContactResolveJob.h"
#include "DetectCollisionJob.h"
#include "RenderJob.h"
#include "UpdateJob.h"

std::mutex renderer::_gameMutex;
renderer::renderer(int screenWidth, int screenHeight, HWND hwnd) : m_initialization_failed_(false), m_game_object_factory_(nullptr), m_physics_manager_(nullptr), m_pause_simulation_(false), m_time_scale_(1), m_total_spheres_in_system_(0), m_number_of_spheres_to_add_(20), m_sphere_diameter_(1), m_friction_(0.4f), m_restitution_(0.4f), m_object_(nullptr)
{
	//Create teams to add be able to add jobs to the Job System
	if (!jobs_sys::the_job_system.create_team("PhysicsTeam", 1, 2, 0))
	{
		std::cout << "Failed";
	}
	if (!jobs_sys::the_job_system.create_team("DetectionTeam", 1, 4, 3))
	{
		std::cout << "Failed";
	}
	if (!jobs_sys::the_job_system.create_team("ResponseTeam", 1, 5, 4))
	{
		std::cout << "Failed";
	}
	if (!jobs_sys::the_job_system.create_team("UpdateTeam", 1, 6, 5))
	{
		std::cout << "Failed";
	}
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);
	//Light colour is set to red

	m_game_object_factory_ = std::make_unique<game_object_factory>(m_game_objects_);
	float xstartPos = -8;
	float ystartPos = 50;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(xstartPos, ystartPos, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(.3f, .3f, .3f), XMFLOAT3(), XMFLOAT3(), collider_component::collider_type::peg, object::object_type::peg, false, 0.2f, 0, 0.2f);
			xstartPos += 3;
		}

		xstartPos = -8;
		ystartPos -= 5;
	}
	xstartPos = -6.5;
	ystartPos = 47.5;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(xstartPos, ystartPos, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(.3f, 0.3f, .3f), XMFLOAT3(), XMFLOAT3(), collider_component::collider_type::peg, object::object_type::peg, false, 0.2f, 0, 0.2f);
			xstartPos += 3;
		}

		xstartPos = -6.5;
		ystartPos -= 5;
	}
	//Left Side
	m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(-10.0f, 25.f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 30.0f, 3.f), XMFLOAT3(), XMFLOAT3(), collider_component::collider_type::aabb_cube, object::object_type::cube, false, 0.2f, 0.2, 0);
	//Right Side
	m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(10.0f, 25.f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 30.0f, 3.f), XMFLOAT3(), XMFLOAT3(), collider_component::collider_type::aabb_cube, object::object_type::cube, false, 0.2f, 0.2, 0);
	//Obb Objects
	m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(9.5f, 65.f, 0.0f), XMFLOAT3(0.0f, 0.0f, -XM_PI / 4), XMFLOAT3(0.3f, 10.f, 0.1f), XMFLOAT3(), XMFLOAT3(), collider_component::collider_type::obb_cube, object::object_type::cube, false, 0.2f, 0.2, 0);
	m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(-9.5f, 65.f, 0.0f), XMFLOAT3(0.0f, 0.0f, XM_PI / 4), XMFLOAT3(0.3f, 10.f, 0.1f), XMFLOAT3(), XMFLOAT3(), collider_component::collider_type::obb_cube, object::object_type::cube, false, 0.2f, 0.2, 0);

	////Bottom
	m_game_objects_.push_back(new game_object(hwnd));
	m_game_objects_.back()->add_transform_component(XMFLOAT3(15.0f, 0.2f, .5f));
	m_game_objects_.back()->add_object_type(object::object_type::cube);
	m_game_objects_.back()->add_collider_component(collider_component::collider_type::plane);
	m_game_objects_.back()->set_plane_collider_data(XMFLOAT3(0.f, 0.0f, 0.f), XMFLOAT3(0.9f, 0.0f, 0.f), XMFLOAT3(0.f, 0.0f, 0.9f), -1.f);
	m_game_objects_.back()->add_rigid_body_component(false, 0.f, 0.f, 0.f, XMFLOAT3(0.0f, -0.0f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(), XMFLOAT3());
	////Invisible Walls
	//m_gameObjectFactory->AddGameObject(hwnd, XMFLOAT3(0.0f, 20.0f, -1.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(50.0f, 100.f, 0.1f), XMFLOAT3(0, 0, 0), XMFLOAT3(), Collider::ColliderType::AABBCube, Object::ObjectType::Collider, false, 1.0f, 0.3, 0);

	//m_gameObjectFactory->AddGameObject(hwnd, XMFLOAT3(0.0f, 20.0f, 1.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(50.0f, 100.f, 0.1f), XMFLOAT3(0, 0, 0), XMFLOAT3(), Collider::ColliderType::AABBCube, Object::ObjectType::Collider, false, 1.f, 0.3, 0);


	float startPos = -10.f;
	for (auto i = 0; i < 7; i++)
	{
		m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(startPos, 1.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT3(0.2f, 7.0f, 0.5f), XMFLOAT3(), XMFLOAT3(),
			collider_component::collider_type::aabb_cube, object::object_type::cube,
			false, 1.f, 0, 0);
		startPos += 3.5f;
	}

	m_physics_manager_ = std::make_unique<physics_manager>(m_game_objects_);
	m_collision_manager_ = std::make_unique<collision_manager>(m_game_objects_, m_friction_, m_restitution_);
	m_resolution_manager_ = std::make_unique<response_manager>(m_collision_manager_->get_contact_manifold_ref(), 50, 50, 0.01f, 0.01f);

	//Clear accumulated forces on each game objects rigid body and calculate all new transformation data.
	for (auto game_object : m_game_objects_)
	{
		game_object->get_rigid_body_component()->clear_accumulators();
		game_object->get_rigid_body_component()->calculate_derived_data();
	}
	console_update();
}

renderer::~renderer()
{
	//Release Objects
	for (auto* gameObject : m_game_objects_)
	{
		if (gameObject)
		{
			delete gameObject;
			gameObject = nullptr;
		}
	}
}
void renderer::pause_simulation()
{
	m_pause_simulation_ = !m_pause_simulation_;
}


void renderer::clear_spheres_()
{
	for (unsigned i = 0; i < m_game_objects_.size(); i++)
	{
		if (m_game_objects_[i]->get_rigid_body_component()->get_use_gravity())
		{
			delete m_game_objects_[i];
			m_game_objects_.at(i) = nullptr;
			m_game_objects_.erase(m_game_objects_.begin() + i);
			--i;
		}
	}

	m_total_spheres_in_system_ = 0;
}
void renderer::add_number_of_spheres(const HWND hwnd)
{
	const XMFLOAT3 startPosition(-10.5f, 70.75f, 0.0f);
	const auto distributionX = abs(startPosition.x * 2) / 7;

	int totalCount = 0;
	unsigned xCount = 1;
	unsigned yCount = 1;

	constexpr int i = -13;
	int x = 0;
	while (x < m_number_of_spheres_to_add_)
	{
		if (xCount == 9)
		{
			xCount = 1;
			yCount++;
		}
		m_game_object_factory_->add_game_object(hwnd, XMFLOAT3(startPosition.x + (xCount * distributionX), startPosition.y + (yCount * distributionX), 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(m_sphere_diameter_ / 2, m_sphere_diameter_ / 2, m_sphere_diameter_ / 2), XMFLOAT3(0.0, 0.1, 0), XMFLOAT3(), collider_component::collider_type::sphere, object::object_type::sphere, true, 0.5f, 0.2f, 0.f);
		xCount++;
		x++;
		totalCount++;
	}

	m_total_spheres_in_system_ += m_number_of_spheres_to_add_;

	//For each gameObject that has a rigidbody we clear the accumulated force and calculate the derived data for the transform matrix (translation and rotation matrix)
	for (const game_object* game_object : m_game_objects_)
	{
		game_object->get_rigid_body_component()->clear_accumulators();
		game_object->get_rigid_body_component()->calculate_derived_data();
	}
	console_update();
}

void renderer::add_time_scale(const int number)
{
	m_time_scale_ += number;

	if (m_time_scale_ < 1)
	{
		m_time_scale_ = 1;
	}

	console_update();
}
void renderer::add_sphere_diameter(const float diameter)
{
	m_sphere_diameter_ += diameter;

	if (m_sphere_diameter_ < 0.1f)
	{
		m_sphere_diameter_ = 0.1f;
	}

	if (m_sphere_diameter_ > 0.9f)
	{
		m_sphere_diameter_ = 0.9f;
	}

	console_update();
}
//
void renderer::add_friction(const float friction)
{
	m_friction_ += friction;

	if (m_friction_ < 0.0f)
	{
		m_friction_ = 0.0f;
	}

	m_collision_manager_->set_friction(m_friction_);
	console_update();
}
//
void renderer::add_restitution(const float restitution)
{
	m_restitution_ += restitution;

	if (m_restitution_ < 0.0f)
	{
		m_restitution_ = 0.0f;
	}

	if (m_restitution_ > 1.0f)
	{
		m_restitution_ = 1.0f;
	}

	m_collision_manager_->set_restitution(m_restitution_);
	console_update();

}

bool renderer::loop()
{
	std::lock_guard<std::mutex> lock(_gameMutex);
	timing::TheTime.update();
	m_dt_ = timing::TheTime.get_delta_time();
	//m_start = Timing::TheTime.GetLastFrameTime();
	//calculate dt based on the simulation loop rate using a timer
	m_dt_ *= static_cast<float>(m_time_scale_);
	if (on == false)
	{
		m_dt_ = 0;
	}
	m_fps_ = static_cast<float>(1.0f / m_dt_);
	//m_physicsManager->CalculateGameObjectPhysics(m_dt);
	calculate_physics_job* physics = new	calculate_physics_job(m_physics_manager_.get(), m_game_objects_);
	jobs_sys::the_job_system.add_job_to_system(physics, "PhysicsTeam");

	m_collision_manager_->dynamic_collision_detection();
	//DetectCollisionJob* detect = new DetectCollisionJob(m_collisionManager);
	//JobsSys::TheJobSystem.AddJobToSystem(detect, "DetectionTeam");

	m_resolution_manager_->contact_resolve(m_dt_);
	//ContactResolveJob* contact = new ContactResolveJob(m_resolutionManager);
	//JobsSys::TheJobSystem.AddJobToSystem(contact, "ResponseTeam");

	//m_physicsManager->UpdateGameObjectPhysics();
	update_job* update = new update_job(m_physics_manager_.get());
	jobs_sys::the_job_system.add_job_to_system(update, "UpdateTeam");

	//Render the graphics scene
	auto const result = render();
	//RenderJob* render = new RenderJob(m_gameObjects);
	//JobsSys::TheJobSystem.AddJobToSystem(render, "RenderTeam");
	return result;
}

bool renderer::render() {

	for (auto* GO : m_game_objects_)
	{
		GO->render();
	}
	return true;
}


bool renderer::get_initialization_state() const {
	return m_initialization_failed_;
}
void renderer::console_update()
{
	system("cls");

	cout << " Number of balls in system: " << m_total_spheres_in_system_ << endl;
	cout << " Friction: " << m_friction_ << endl;
	cout << " Restitution: " << m_restitution_ << endl << endl;
	cout << " 1 - Add Number of Spheres: " << endl;
	cout << " R - Reset System" << endl;
	cout << " P - Toggle Pause Simulation" << endl;
	cout << " U, J - Increase/Decrease TimeScale: " << m_time_scale_ << endl;
	cout << " T, B - Increase/Decrease Sphere Diameter: " << m_time_scale_ << endl;
	cout << " I, K - Increase/Decrease Friction: " << m_friction_ << endl;
	cout << " O, L - Increase/Decrease Restitution: " << m_restitution_ << endl << endl;
	cout << " W, S, A, D - Up, Down, Left, Right Camera Controls" << endl;
	cout << " Up, Down Arrow - Zoom In/Out" << endl;
}