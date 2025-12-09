#include "stdafx.h"
#include "TitleCamera.h"

bool TitleCamera::Start()
{
	g_camera3D->SetPosition(Vector3{ 0.0f, 5000.0f, -17500.0f });
	g_camera3D->SetTarget(Vector3{ 0.0f, 5000.0f, 0.0f });
	return true;
}