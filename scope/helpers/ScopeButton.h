#pragma once

namespace scope {

	/** Mimicks a button, glue between the GUI and controller, used in ScopeController.
	* Could have used a ScopeNumber<bool>?! */
	class ScopeButton {

		protected:
			/** name of the control */
			std::wstring name;

			/** enabled/disabled status */
			bool enabled;

			/** typedef for button click signal */
			typedef boost::signals2::signal<void ()> signalchange_t;

			/** typedef for control state */
			typedef boost::signals2::signal<void (bool)> signalstate_t;

			/** button click signal */
			signalchange_t m_changesig;

			/** control state signal */
			signalstate_t m_ctrlstatesig;

		public:
			/** Construct and enable */
			ScopeButton(const std::wstring& _name = L"None");

			/** Copy constructor */
			ScopeButton(const ScopeButton& sb);

			ScopeButton& operator=(const ScopeButton& sb);

			/** @return name of button */
			std::wstring Name() const;

			/** @return true if currently enabled */
			bool Enabled() const { return enabled; }

			/** Enable/disable button */
			void Enable(const bool& state);

			/** Call signal */
			void Click();

			/** @name Handles connections to the signal for click and control state
			* @{ */
			/** Connect slot to signal */
			boost::signals2::connection Connect(signalchange_t::slot_type slot);

			/** Connect slot to control state signal */
			boost::signals2::connection ConnectControlState(signalstate_t::slot_type slot);

			/** Disconnect slot from signal */
			void Disconnect(signalchange_t::slot_type slot);

			/** Disconnect slot from control state signal */
			void DisconnectControlState(signalstate_t::slot_type slot);
			/** @} */
	};

}