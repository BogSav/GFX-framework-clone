#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"


namespace m2
{
	class Tema2 : public gfxc::SimpleScene
	{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void OpenDialog();
		void OnFileSelected(const std::string& fileName);

		// Processing effects
		void GrayScale();
		char GetGrayValue(glm::vec3 color);
		void Removel();
		void Sobel(Texture2D* sourceTexture, Texture2D* destinationTexture, float threshold) const;
		std::vector<uint8_t> Sobel(Texture2D* sourceTexture, float threshold) const;
		void SaveImage(const std::string& fileName);

	private:
		Texture2D* originalImage;
		Texture2D* watermarkImage;
		Texture2D* processedImage;

		int outputMode;
		bool gpuProcessing;
		bool saveScreenToImage;
	};
}   // namespace m2
