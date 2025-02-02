#pragma once

#include "oom/oom.hpp"
#include "Engine/Renderer/Camera.h"

namespace engine
{

	enum class CameraMode
	{
		NONE, ORTHO, FLYCAM, ARCBALL
	};

	class EditorCamera : public Camera
	{

	public:
		static std::unique_ptr<engine::EditorCamera> g_editorCam;

		EditorCamera();
		EditorCamera(const oom::mat4& projectionMatrix);

		void Focus(const oom::vec3& focusPoint);
		void OnUpdate(float dt);

		void  SetViewportSize(uint32_t width, uint32_t height) { m_viewportWidth = width; m_viewportHeight = height; }
		float GetDistance() const { return m_distance; }
		void  SetDistance(float distance) { m_distance = distance; }

		const oom::vec3 GetUp() const;
		const oom::vec3 GetRight() const;
		const oom::vec3 GetForward() const;
		const oom::vec3& GetPosition() const { return m_position; }

		void UpdateScroll(float mWheelY);

		/*void MoveForwards();
		void MoveBackwards();
		void MoveRight();
		void MoveLeft();
		void MoveUp();
		void MoveDown();*/

		oom::mat4 GetViewProj() const { return m_projection * m_view; }
		const oom::mat4& GetView() const { return m_view; }

	private:
		void UpdateView();
		void UpdateProjection();

		//Camera movements
		void MousePan(const oom::vec2& delta);


		bool m_cameraEnabled{};
		CameraMode m_cameraMode{ CameraMode::ORTHO };

		oom::mat4 m_view{};
		oom::vec3 m_position{};

		oom::vec3 m_focalPoint{};

		float m_distance;

		float m_panSpeed{ 500 };
		oom::vec2 m_lastMousePosition;
		uint32_t m_viewportWidth{ 1600 }, m_viewportHeight{ 900 };

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = oom::radians(45.0f);
		float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 500.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
	};


}
