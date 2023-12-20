#pragma once

#include <astra/common.h>
#include <astra/window.h>

namespace Astra {

    class Application
    {
    public:
        /**
         * @brief Constructor
         */
        Application();

        /**
         * @brief Destructor
         */
        ~Application();

        /**
         * @brief Main run loop,
         * which handles window events and updates it.
         */
        void Run();

        /**
         * @brief Closes the application.
         */
        void Close();

    public:

        /**
         * @brief Callback function to process messages for the specified window.
         *
         * This static function is used as a callback to process messages for the given window.
         *
         * @param hwnd The handle to the window.
         * @param message The message identifier.
         * @param wParam Additional message information.
         * @param lParam Additional message information.
         * @return The result of the message processing and depends on the message sent.
         */
        static LRESULT CALLBACK ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        /**
         * @brief Gets the instance of the Application.
         * @return Reference to the Application instance.
         */
        static Application& Get();

    private:

        static Application* s_Instance;
        static WideChar s_WindowClass[];

    private:

        HINSTANCE m_InstanceHandle;
        Unique<Window> m_Window;
        bool m_Running;
    };
}
