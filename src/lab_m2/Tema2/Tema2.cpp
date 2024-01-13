#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>
#include <algorithm>

#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;

#include <chrono>


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
	outputMode = 0;
	gpuProcessing = false;
	saveScreenToImage = false;
	//window->SetSize(600, 600);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
	// Load default texture fore imagine processing
	originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEMA2, "star.png"), nullptr, "image", true, true);
	watermarkImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEMA2, "watermark.png"), nullptr, "watermark", true, true);
	processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEMA2, "star.png"), nullptr, "procImage", true, true);

	window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());

	{
		Mesh* mesh = new Mesh("quad");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
		mesh->UseMaterials(false);
		meshes[mesh->GetMeshID()] = mesh;
	}

	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema2", "shaders");

	// Create a shader program for particle system
	{
		Shader* shader = new Shader("ImageProcessing");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}


void Tema2::FrameStart()
{
}


void Tema2::Update(float deltaTimeSeconds)
{
	ClearScreen();

	auto shader = shaders["ImageProcessing"];
	shader->Use();

	if (saveScreenToImage)
	{
		window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
	}

	int flip_loc = shader->GetUniformLocation("flipVertical");
	glUniform1i(flip_loc, saveScreenToImage ? 0 : 1);

	int screenSize_loc = shader->GetUniformLocation("screenSize");
	glm::ivec2 resolution = window->GetResolution();
	glUniform2i(screenSize_loc, resolution.x, resolution.y);

	int outputMode_loc = shader->GetUniformLocation("outputMode");
	glUniform1i(outputMode_loc, outputMode);

	int locTexture = shader->GetUniformLocation("textureImage");
	glUniform1i(locTexture, 0);

	auto textureImage = processedImage;
	textureImage->BindToTextureUnit(GL_TEXTURE0);

	RenderMesh(meshes["quad"], shader, glm::mat4(1));

	if (saveScreenToImage)
	{
		saveScreenToImage = false;

		GLenum format = GL_RGB;
		if (originalImage->GetNrChannels() == 4)
		{
			format = GL_RGBA;
		}

		glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), format, GL_UNSIGNED_BYTE, processedImage->GetImageData());
		processedImage->UploadNewData(processedImage->GetImageData());
		SaveImage("shader_processing_" + std::to_string(outputMode));

		float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
		window->SetSize(static_cast<int>(600 * aspectRatio), 600);
	}
}


void Tema2::FrameEnd()
{
	DrawCoordinateSystem();
}


void Tema2::OnFileSelected(const std::string& fileName)
{
	if (fileName.size())
	{
		std::cout << fileName << endl;
		originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
		processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

		float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
		window->SetSize(static_cast<int>(600 * aspectRatio), 600);
	}
}


void Tema2::GrayScale()
{
	unsigned int channels = originalImage->GetNrChannels();
	unsigned char* data = originalImage->GetImageData();
	unsigned char* newData = processedImage->GetImageData();

	if (channels < 3)
		return;

	glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());

	for (int i = 0; i < imageSize.y; i++)
	{
		for (int j = 0; j < imageSize.x; j++)
		{
			int offset = channels * (i * imageSize.x + j);

			// Reset save image data
			char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07);
			memset(&newData[offset], value, 3);
		}
	}

	processedImage->UploadNewData(newData);
}

char m2::Tema2::GetGrayValue(glm::vec3 color)
{
	return  static_cast<char>(color.x * 0.2f + color.y * 0.71f + color.z * 0.07);
}

void m2::Tema2::Sobel(Texture2D* sourceTexture, Texture2D* destinationTexture, float threshold) const
{
	const unsigned int channels = sourceTexture->GetNrChannels();
	const unsigned char* data = sourceTexture->GetImageData();

	if (channels != destinationTexture->GetNrChannels())
	{
		throw std::exception("Dfk??");
	}

	unsigned char* newData = destinationTexture->GetImageData();

	std::vector<glm::vec3> Gx(3);
	Gx[0] = glm::vec3{ -1, 0, 1 };
	Gx[1] = glm::vec3(-2, 0, 2);
	Gx[2] = glm::vec3(-1, 0, 1);

	std::vector<glm::vec3> Gy(3);
	Gy[0] = glm::vec3(1, 2, 1);
	Gy[1] = glm::vec3(0, 0, 0);
	Gy[2] = glm::vec3(-1, -2, -1);

	for (size_t k = 1; k < sourceTexture->GetWidth() - 1; k++)
	{
		for (size_t l = 1; l < sourceTexture->GetHeight() - 1; l++)
		{
			int offset = channels * (l * sourceTexture->GetWidth() + k);

			float sumX = 0.f;
			float sumY = 0.f;

			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int localOffset = channels * ((l + i) * sourceTexture->GetWidth() + k + j);
					char gray = static_cast<char>(data[localOffset + 0] * 0.2f + data[localOffset + 1] * 0.71f + data[localOffset + 2] * 0.07);

					sumX += gray * Gx[i + 1][j + 1];
					sumY += gray * Gy[i + 1][j + 1];
				}
			}

			float magnitude = std::sqrt(sumX * sumX + sumY * sumY) / 255.f;

			if (magnitude < threshold)
			{
				memset(&newData[offset], 0, 3);
			}
			else
			{
				newData[offset] = 255;
				newData[offset + 1] = 255;
				newData[offset + 2] = 255;
			}
		}
	}

	destinationTexture->UploadNewData(newData);
}

std::vector<uint8_t> m2::Tema2::Sobel(Texture2D* sourceTexture, float threshold) const
{
	std::vector<uint8_t> sobelVector(sourceTexture->GetWidth() * sourceTexture->GetHeight(), 0);

	const unsigned int channels = sourceTexture->GetNrChannels();
	const unsigned char* data = sourceTexture->GetImageData();

	std::vector<glm::vec3> Gx(3);
	Gx[0] = glm::vec3{ -1, 0, 1 };
	Gx[1] = glm::vec3(-2, 0, 2);
	Gx[2] = glm::vec3(-1, 0, 1);

	std::vector<glm::vec3> Gy(3);
	Gy[0] = glm::vec3(1, 2, 1);
	Gy[1] = glm::vec3(0, 0, 0);
	Gy[2] = glm::vec3(-1, -2, -1);

	for (size_t k = 1; k < sourceTexture->GetWidth() - 1; k++)
	{
		for (size_t l = 1; l < sourceTexture->GetHeight() - 1; l++)
		{
			float sumX = 0.f;
			float sumY = 0.f;

			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int localOffset = channels * ((l + j) * sourceTexture->GetWidth() + k + i);
					char gray = static_cast<char>(data[localOffset + 0] * 0.2f + data[localOffset + 1] * 0.71f + data[localOffset + 2] * 0.07);

					sumX += gray * Gx[i + 1][j + 1];
					sumY += gray * Gy[i + 1][j + 1];
				}
			}

			float magnitude = std::sqrt(sumX * sumX + sumY * sumY) / 255.f;
			int offset = l * sourceTexture->GetWidth() + k;

			if (magnitude < threshold)
			{
				sobelVector[offset] = 0;
			}
			else
			{
				sobelVector[offset] = 1;
			}
		}
	}

	return sobelVector;
}

void m2::Tema2::Removel()
{
	std::vector<uint8_t> sobelWater = Sobel(watermarkImage, 0.9);
	std::vector<uint8_t> sobelOriginal = Sobel(originalImage, 0.9);

	size_t whiteCount = std::count(sobelWater.begin(), sobelWater.end(), 1);

	unsigned char* newData = processedImage->GetImageData();
	unsigned char* watermarkData = watermarkImage->GetImageData();

	const unsigned int imageHeight = originalImage->GetHeight();
	const unsigned int imageWidth = originalImage->GetWidth();
	const unsigned int watermarkWidth = watermarkImage->GetWidth();
	const unsigned int watermarkHeight = watermarkImage->GetHeight();

	const unsigned int waterPixelCount = watermarkHeight * watermarkWidth;

	std::vector<glm::ivec2> startPosition;

	// Detectam watermark-urile
	for (size_t i = 0; i < imageWidth - watermarkWidth; i++)
	{
		for (size_t j = 0; j < imageHeight - watermarkHeight; j++)
		{
			int nrOfCorrespondingPixels = 0;

			for (size_t k = 0; k < watermarkWidth; k++)
			{
				for (size_t l = 0; l < watermarkHeight; l++)
				{
					int imageOffset = (j + l) * imageWidth + i + k;
					int waterOffset = l * watermarkWidth + k;

					if (sobelWater[waterOffset] == 1 && sobelOriginal[imageOffset] == 1)
						nrOfCorrespondingPixels++;
				}
			}

			if ((float)nrOfCorrespondingPixels / (float)whiteCount > 0.9f)
			{
				std::cout << "Gasit" << std::endl;
				startPosition.push_back(glm::ivec2(i, j));
			}
		}
	}

	for (glm::ivec2& corner : startPosition)
	{
		for (size_t i = 0; i < watermarkWidth; i++)
		{
			for (size_t j = 0; j < watermarkHeight; j++)
			{
				int waterOffset = watermarkImage->GetNrChannels() * (j * watermarkWidth + i);
				int offset = (corner.y + j) * imageWidth + corner.x + i;
				offset *= processedImage->GetNrChannels();
				newData[offset] -= watermarkData[waterOffset];
				newData[offset + 1] -= watermarkData[waterOffset + 1];
				newData[offset + 2] -= watermarkData[waterOffset + 2];
			}
		}
	}

	processedImage->UploadNewData(newData);
}


void Tema2::SaveImage(const std::string& fileName)
{
	cout << "Saving image! ";
	processedImage->SaveToFile((fileName + ".png").c_str());
	cout << "[Done]" << endl;
}


void Tema2::OpenDialog()
{
	std::vector<std::string> filters =
	{
		"Image Files", "*.png *.jpg *.jpeg *.bmp",
		"All Files", "*"
	};

	auto selection = pfd::open_file("Select a file", ".", filters).result();
	if (!selection.empty())
	{
		std::cout << "User selected file " << selection[0] << "\n";
		OnFileSelected(selection[0]);
	}
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// Treat continuous update based on input
}


void Tema2::OnKeyPress(int key, int mods)
{
	// Add key press event
	if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
	{
		OpenDialog();
	}

	if (key == GLFW_KEY_E)
	{
		gpuProcessing = !gpuProcessing;
		if (gpuProcessing == false)
		{
			outputMode = 0;
		}
		cout << "Processing on GPU: " << (gpuProcessing ? "true" : "false") << endl;
	}

	if (key - GLFW_KEY_0 >= 0 && key < GLFW_KEY_5)
	{
		outputMode = key - GLFW_KEY_0;

		if (gpuProcessing == false)
		{
			auto start = std::chrono::high_resolution_clock::now();

			if (outputMode == 1)
			{
				GrayScale();
			}
			if (outputMode == 2)
			{
				Sobel(originalImage, processedImage, 0.9f);
			}
			if (outputMode == 3)
			{
				Removel();
			}

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = end - start;
			std::cout << "Durata: " << duration.count() << " secunde" << std::endl;
		}
	}

	if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
	{
		if (!gpuProcessing)
		{
			SaveImage("processCPU_" + std::to_string(outputMode));
		}
		else {
			saveScreenToImage = true;
		}
	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// Treat mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
	// Treat window resize event
}
