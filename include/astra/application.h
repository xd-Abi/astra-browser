#pragma once

#include <astra/common.h>

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
        bool m_Running;
    };
}
