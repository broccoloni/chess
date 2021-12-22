#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
	public:
		Camera2D();
		~Camera2D();
	
		void init(int m_screenWidth, int m_screenHeight);	
		void update();

		void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsMatrixUpdate = true;}
		void setScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

		glm::vec2 getPosition() { return m_position; }
		float getScale() { return m_scale; }
		glm::mat4 getCameraMatrix() { return m_cameraMatrix; }

		glm::vec2 screenCoordsToWorld(glm::vec2 screenCoords);

	private:
		int m_screenWidth, m_screenHeight;
		bool m_needsMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix;
		glm::mat4 m_orthoMatrix;

};




