#pragma once

#include "..\MessageBus.hpp"
#include "..\..\Window\Window.hpp"

#include <mutex>

namespace Magma
{
	/// <summary>
	///		Input axis class.
	///		Used to create a layer of abstraction between the keyboard/controllers raw input and the engine controls.
	/// </summary>
	class InputAxis : public Serializable
	{
	public:
		/// <summary>
		///		Gets this axis value
		/// </summary>
		/// <returns>Axis value</returns>
		inline float Value() const { std::lock_guard<std::mutex> lockGuard(m_mutex); return m_value; }

		/// <summary>
		///		Sets the positive mouse button for this axis (set to Mouse::Button::Invalid for none)
		/// </summary>
		/// <param name="mb">Positive mouse button</param>
		inline void SetPositiveMouseButton(Mouse::Button mb) { std::lock_guard<std::mutex> lockGuard(m_mutex); m_positiveMouseButton = mb; }

		/// <summary>
		///		Sets the negative mouse button for this axis (set to Mouse::Button::Invalid for none)
		/// </summary>
		/// <param name="mb">Negative mouse button</param>
		inline void SetNegativeMouseButton(Mouse::Button mb) { std::lock_guard<std::mutex> lockGuard(m_mutex); m_negativeMouseButton = mb; }

		/// <summary>
		///		Sets the positive keyboard key for this axis (set to Keyboard::Key::Invalid for none)
		/// </summary>
		/// <param name="mb">Positive keyboard key</param>
		inline void SetPositiveKey(Keyboard::Key key) { std::lock_guard<std::mutex> lockGuard(m_mutex); m_positiveKey = key; }

		/// <summary>
		///		Sets the negative keyboard key for this axis (set to Keyboard::Key::Invalid for none)
		/// </summary>
		/// <param name="mb">Negative keyboard key</param>
		inline void SetNegativeKey(Keyboard::Key key) { std::lock_guard<std::mutex> lockGuard(m_mutex); m_negativeKey = key; }

		/// <summary>
		///		Sets the speed of this input axis (speed at which the value adjusts to the raw input)
		/// </summary>
		/// <param name="decaySpeed">New speed</param>
		inline void SetSpeed(float speed) { std::lock_guard<std::mutex> lockGuard(m_mutex); m_speed = speed; }

		/// <summary>
		///		Sets the decay speed of this input axis (speed at which the value returns to zero)
		/// </summary>
		/// <param name="decaySpeed">New decay speed</param>
		inline void SetDecaySpeed(float decaySpeed) { std::lock_guard<std::mutex> lockGuard(m_mutex); m_decaySpeed = decaySpeed; }

	private:
		friend class Input;

		float m_value = 0.0f;
		float m_speed = 1.0f;
		float m_decaySpeed = 1.0f;

		Mouse::Button m_positiveMouseButton = Mouse::Button::Invalid;
		Mouse::Button m_negativeMouseButton = Mouse::Button::Invalid;
		Keyboard::Key m_positiveKey = Keyboard::Key::Invalid;
		Keyboard::Key m_negativeKey = Keyboard::Key::Invalid;

		mutable std::mutex m_mutex;

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};

	/// <summary>
	///		Input system.
	///		Handles windows and input.
	/// </summary>
	class Input : public MessageListener, public Serializable
	{
	public:
		inline std::shared_ptr<Window> GetWindow() const { return m_window; }
		void SetWindow(std::shared_ptr<Magma::Window> window);

		void Update(float deltaTime);

		InputAxis& operator[](const std::string& axisName);

	private:
		std::map<std::string, InputAxis> m_inputAxes;
		mutable std::mutex m_inputAxes_mutex;

		std::uint64_t m_iCallback;

		std::shared_ptr<Magma::Window> m_window;

		// Inherited via MessageListener
		virtual void DerivedInit() override;
		virtual void DerivedTerminate() override;

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};
}