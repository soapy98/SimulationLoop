#include "ScaleComponent.h"
scale_component::scale_component() : m_scale(XMVECTOR())
{

}
scale_component::scale_component(const XMFLOAT3& scale): m_scale(XMLoadFloat3(&scale)) {
}


