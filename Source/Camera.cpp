//#include "Camera.h"
//
//// 指定方向を向く
//void Camera::SetLookAt(const DirectX::XMFLOAT4& cameraPosition, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
//{
//	// 視点、注視点、上方向からビュー行列を作成
//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat4(&cameraPosition);
//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
//	DirectX::XMStoreFloat4x4(&view, View);
//
//	// ビューを逆行列化し、ワールド行列に戻す
//	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
//	DirectX::XMFLOAT4X4 world;
//	DirectX::XMStoreFloat4x4(&world, World);
//
//	// カメラの方向を取り出す
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
//	// 視点、注視点を保存
//	this->cameraPositon.x = cameraPosition.x;
//	this->cameraPositon.y = cameraPosition.y;
//	this->cameraPositon.z = cameraPosition.z;
//	this->focus.x = focus.x;
//	this->focus.y = focus.y;
//	this->focus.z = focus.z;
//}
//
//// パースペクティブ設定
//void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
//{
//	// 画角、画面比率、クリップ距離からプロジェクション行列を作成
//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
//	DirectX::XMStoreFloat4x4(&projection, Projection);
//
//	this->nearZ = nearZ;
//	this->farZ = farZ;
//}
