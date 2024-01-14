#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>
#include <algorithm>

#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;

const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";

#include <chrono>

Tema2::Tema2()
{
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

	int locTexture = shader->GetUniformLocation("textureImage");
	glUniform1i(locTexture, 0);

	auto textureImage = processedImage;
	textureImage->BindToTextureUnit(GL_TEXTURE0);

	RenderMesh(meshes["quad"], shader, glm::mat4(1));
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

char m2::Tema2::GetGrayValue(const glm::vec3& color) const noexcept
{
	return  static_cast<char>(color.x * 0.2f + color.y * 0.71f + color.z * 0.07);
}

void m2::Tema2::Sobel0(Texture2D* sourceTexture, Texture2D* destinationTexture, float threshold) const
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

std::vector<uint8_t> m2::Tema2::Sobel(Texture2D* sourceTexture, float threshold)
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

void printProgressBar(int percent)
{
	const int barWidth = 50;

	std::cout << "	[";
	std::cout << GREEN;

	int pos = barWidth * percent / 100;

	for (int i = 0; i < barWidth; ++i)
	{
		std::cout << ((i < pos) ? "=" : " ");
	}

	std::cout << RESET << "]";
	std::cout << YELLOW << " " << percent << "%" << RESET << "\r";
	std::flush(std::cout);
}

void m2::Tema2::Removel()
{
	unsigned int numThreads = std::thread::hardware_concurrency();
	std::cout << RED << "Info " << RESET
		<< "Sunt disponibile "
		<< YELLOW
		<< numThreads
		<< RESET
		<< " thread-uri pentru procesare\n";

	// Creare thread pool cu nr de thread-uri disponibile
	tk::ThreadPool pool{ numThreads };

	// Pasul 1: Aplicarea filtrului Sobel pe watermark si pe imaginea originala
	std::cout << RED << "Pas 1 " << RESET << "Aplicarea filtrului Sobel pe imagini\n";

	auto f1 = pool.Run(Sobel, originalImage, 0.9);
	auto f2 = pool.Run(Sobel, watermarkImage, 0.9);
	pool.WaitForAllDone();

	printProgressBar(0);
	std::vector<uint8_t> sobelWater = f2.get();
	printProgressBar(50);
	std::vector<uint8_t> sobelOriginal = f1.get();
	printProgressBar(100);

	std::cout << "\n	Status: " << GREEN << "succes" << RESET << "\n";

	// Preluam datele importante ce vor fi folosit in continuare
	int whiteCount = static_cast<int>(std::count(sobelWater.begin(), sobelWater.end(), 1));

	unsigned char* newData = processedImage->GetImageData();
	unsigned char* originalData = originalImage->GetImageData();
	unsigned char* watermarkData = watermarkImage->GetImageData();

	const int imageHeight = (int)originalImage->GetHeight();
	const int imageWidth = (int)originalImage->GetWidth();
	const int watermarkWidth = (int)watermarkImage->GetWidth();
	const int watermarkHeight = (int)watermarkImage->GetHeight();

	const int totalIterations = (imageWidth - watermarkWidth) * (imageHeight - watermarkHeight);
	int lastPercDisplayed = -1;

	std::vector<std::future<std::optional<glm::ivec2>>> futures;

	// Pasul 2: Identificarea pozitiilor la care se afla watermark-ul
	// Aici doar atribuim task-uri thread-urilor, nu facem mare lucru
	std::cout << RED << "Pas 2 " << RESET << "Identificarea pozitiilor watermark-urilor\n";
	for (int i = 0; i < imageWidth - watermarkWidth; i++)
	{
		for (int j = 0; j < imageHeight - watermarkHeight; j++)
		{
			// Afisam niste procente
			int currentIteration = i * (imageHeight - watermarkHeight) + j;
			int perc = static_cast<int>(100.0 * currentIteration / totalIterations);
			if (perc % 5 == 0 && perc > lastPercDisplayed)
			{
				printProgressBar(perc);
				lastPercDisplayed = perc;
			}

			// Creem un nou future pentru pixelul de la (i, j) cu functia de identificare
			futures.push_back(
				pool.Run(
					CheckForWatermark,
					std::cref(sobelWater),
					std::cref(sobelOriginal),
					i,
					j,
					imageHeight,
					imageWidth,
					watermarkHeight,
					watermarkWidth,
					whiteCount));
		}
	}
	printProgressBar(100);
	std::cout << "\n	Status: " << GREEN << "succes" << RESET << "\n";

	// Pasul 3: Eliminarea efectiva a watermark-urilor din imagine
	// Aici vom prelua rezultatele worker-ilor, timp in care vom si elimina pe main-thread
	// wateramrk-urile imediat ce sunt gasit, astfel oferim timp celorlalte thread-uri sa isi termine
	// treaba
	std::cout << RED << "Pas 3 " << RESET << "Eliminarea wateramrk-urilor din imagine\n";

	std::vector<glm::ivec2> startPositions;
	int index = 0;
	lastPercDisplayed = -1;

	for (auto& f : futures)
	{
		try
		{
			int perc = static_cast<int>(100.f * index++ / futures.size());
			if (perc % 5 == 0 && perc > lastPercDisplayed)
			{
				printProgressBar(perc);
				lastPercDisplayed = perc;
			}

			auto res = f.get();
			if (res.has_value())
			{
				glm::ivec2 corner = res.value();

				// Watermark remove din imagine
				for (size_t i = 0; i < watermarkWidth; i++)
				{
					for (size_t j = 0; j < watermarkHeight; j++)
					{
						int waterOffset = watermarkImage->GetNrChannels() * (j * watermarkWidth + i);
						int offset = processedImage->GetNrChannels() * ((corner.y + j) * imageWidth + corner.x + i);

						newData[offset] = originalData[offset] - watermarkData[waterOffset];
						newData[offset + 1] = originalData[offset + 1] - watermarkData[waterOffset + 1];
						newData[offset + 2] = originalData[offset + 2] - watermarkData[waterOffset + 2];
					}
				}
			}
		}
		catch (...)
		{
			std::cout << "Eroare grava" << std::endl;
		}
	}

	printProgressBar(100);
	std::cout << "\n	Status: " << GREEN << "succes" << RESET << "\n";
	std::cout << RED << "Info " << RESET << "Procesarea s-a incheiat - watermark-uri eliminate cu " << GREEN << "succes" << RESET << std::endl;

	processedImage->UploadNewData(newData);
}

std::optional<glm::ivec2> m2::Tema2::CheckForWatermark(
	const std::vector<uint8_t>& sobelWater,
	const std::vector<uint8_t>& sobelOriginal,
	const int& i,
	const int& j,
	const int& imageHeight,
	const int& imageWidth,
	const int& watermarkHeight,
	const int& watermarkWidth,
	const int& whiteCount)
{
	int nrOfCorrespondingPixels = 0;

	for (int k = 0; k < watermarkWidth; k++)
	{
		for (int l = 0; l < watermarkHeight; l++)
		{
			int imageOffset = (j + l) * imageWidth + i + k;
			int waterOffset = l * watermarkWidth + k;

			if (sobelWater[waterOffset] == 1 && sobelOriginal[imageOffset] == 1)
				nrOfCorrespondingPixels++;
		}

		// 3037 white dots => around 303.7 white dots per letter
		// in first 130 pixel on width there are 3 letters => should be around 909 white pixels 
		// deci statistic vorbind relatia de mai jos ar trebui sa dea un early reject

		if (k > watermarkWidth / 2 && nrOfCorrespondingPixels < whiteCount / 3)
			return {};
	}

	if ((float)nrOfCorrespondingPixels / (float)whiteCount > 0.85f)
	{
		return glm::ivec2(i, j);
	}
	else
	{
		return {};
	}
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

	if (key - GLFW_KEY_0 >= 0 && key < GLFW_KEY_5)
	{
		int outputMode = key - GLFW_KEY_0;

		auto start = std::chrono::high_resolution_clock::now();
		if (outputMode == 1)
		{
			GrayScale();
		}
		if (outputMode == 2)
		{
			Sobel0(originalImage, processedImage, 0.1f);
		}
		if (outputMode == 3)
		{
			Removel();
		}
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> duration = end - start;
		std::cout << "Durata totala: " << YELLOW << duration.count() << RESET << " secunde\n\n";
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
