#include "PositionComponent.h"

position_component::position_component() : m_position(0.0f, 0.0f, 0.0f), m_new_position_(0.0f, 0.0f, 0.0f)
{
}

position_component::position_component(const XMFLOAT3 position) : m_position(position), m_new_position_(0.0f, 0.0f, 0.0f) {

}

position_component::position_component(const float x, const float y, const float z) : m_position(x, y, z), m_new_position_(0.0f, 0.0f, 0.0f){

}