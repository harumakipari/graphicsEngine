//#include "Camera.h"
//
//// �w�����������
//void Camera::SetLookAt(const DirectX::XMFLOAT4& cameraPosition, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
//{
//	// ���_�A�����_�A���������r���[�s����쐬
//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat4(&cameraPosition);
//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
//	DirectX::XMStoreFloat4x4(&view, View);
//
//	// �r���[���t�s�񉻂��A���[���h�s��ɖ߂�
//	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
//	DirectX::XMFLOAT4X4 world;
//	DirectX::XMStoreFloat4x4(&world, World);
//
//	// �J�����̕��������o��
//	this->right.x = world._11;
//	this->right.y = world._12;
//	this->right.z = world._13;
//
//	this->up.x = world._21;
//	this->up.y = world._22;
//	this->up.z = world._23;
//
//	this->front.x = world._31;
//	this->front.y = world._32;
//	this->front.z = world._33;
//
//	// ���_�A�����_��ۑ�
//	this->cameraPositon.x = cameraPosition.x;
//	this->cameraPositon.y = cameraPosition.y;
//	this->cameraPositon.z = cameraPosition.z;
//	this->focus.x = focus.x;
//	this->focus.y = focus.y;
//	this->focus.z = focus.z;
//}
//
//// �p�[�X�y�N�e�B�u�ݒ�
//void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
//{
//	// ��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
//	DirectX::XMStoreFloat4x4(&projection, Projection);
//
//	this->nearZ = nearZ;
//	this->farZ = farZ;
//}
