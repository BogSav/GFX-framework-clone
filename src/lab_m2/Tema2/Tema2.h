#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"

#include <ranges>
#include <condition_variable>
#include <functional>
#include <deque>
#include <optional>
#include <semaphore>
#include <cassert>
#include <ranges>
#include <variant>
#include <future>
#include <random>
#include <functional>
#include <array>
#include <ranges>
#include <cmath>
#include <numbers>

namespace rn = std::ranges;
namespace vi = rn::views;

namespace tk
{
	class ThreadPool
	{
		using Task = std::move_only_function<void()>;
	public:
		ThreadPool(size_t numWorkers)
		{
			workers_.reserve(numWorkers);
			for (size_t i = 0; i < numWorkers; i++) {
				workers_.emplace_back(this);
			}
		}
		template<typename F, typename...A>
		auto Run(F&& function, A&&...args)
		{
			using ReturnType = std::invoke_result_t<F, A...>;
			auto pak = std::packaged_task<ReturnType()>{ std::bind(
				std::forward<F>(function), std::forward<A>(args)...
			) };
			auto future = pak.get_future();
			Task task{ [pak = std::move(pak)]() mutable { pak(); } };
			{
				std::lock_guard lk{ taskQueueMtx_ };
				tasks_.push_back(std::move(task));
			}
			taskQueueCv_.notify_one();
			return future;
		}
		void WaitForAllDone()
		{
			std::unique_lock lk{ taskQueueMtx_ };
			allDoneCv_.wait(lk, [this] {return tasks_.empty(); });
		}
		~ThreadPool()
		{
			for (auto& w : workers_) {
				w.RequestStop();
			}
		}

	private:
		// functions
		Task GetTask_(std::stop_token& st)
		{
			Task task;
			std::unique_lock lk{ taskQueueMtx_ };
			taskQueueCv_.wait(lk, st, [this] {return !tasks_.empty(); });
			if (!st.stop_requested()) {
				task = std::move(tasks_.front());
				tasks_.pop_front();
				if (tasks_.empty()) {
					allDoneCv_.notify_all();
				}
			}
			return task;
		}
		// types
		class Worker_
		{
		public:
			Worker_(ThreadPool* pool) : pool_{ pool }, thread_(std::bind_front(&Worker_::RunKernel_, this)) {}
			void RequestStop()
			{
				thread_.request_stop();
			}
		private:
			// functions
			void RunKernel_(std::stop_token st)
			{
				while (auto task = pool_->GetTask_(st)) {
					task();
				}
			}
			// data
			ThreadPool* pool_;
			std::jthread thread_;
		};
		// data
		std::mutex taskQueueMtx_;
		std::condition_variable_any taskQueueCv_;
		std::condition_variable allDoneCv_;
		std::deque<Task> tasks_;
		std::vector<Worker_> workers_;
	};
}

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
		char GetGrayValue(const glm::vec3& color) const noexcept;
		void Removel();
		void Sobel0(Texture2D* sourceTexture, Texture2D* destinationTexture, float threshold) const;
		static std::vector<uint8_t> Sobel(Texture2D* sourceTexture, float threshold);

		static std::optional<glm::ivec2> CheckForWatermark(
			const std::vector<uint8_t>& sobelWater,
			const std::vector<uint8_t>& sobelOriginal,
			const int& i,
			const int& j,
			const int& imageHeight,
			const int& imageWidth,
			const int& watermarkHeight,
			const int& watermarkWidth,
			const int& whiteCount);

	private:
		Texture2D* originalImage;
		Texture2D* watermarkImage;
		Texture2D* processedImage;
	};
}   // namespace m2
