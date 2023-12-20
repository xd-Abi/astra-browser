#pragma once

#include <astra/common.h>

namespace Astra {

    class Window
    {
    public:
        /**
         * @brief Constructor for the Window class.
         * @param instanceHandle The handle to the application instance.
         * @param windowClass The window class name (WideChar) for creating the window.
         */
        Window(HINSTANCE instanceHandle, const WideChar windowClass[]);

        /**
         * @brief Destructor
         */
        ~Window();

        /**
         * @brief Gets the handle of the window.
         * @return The handle of the window (HWND on Windows).
         */
        HWND GetHandle() const;

        /**
         * @brief Gets the width of the window.
         * @return The width of the window.
         */
        uint32 GetWidth() const;

        /**
         * @brief Gets the height of the window.
         * @return The height of the window.
         */
        uint32 GetHeight() const;

    private:
        HWND m_Handle;
        uint32 m_Width;
        uint32 m_Height;
    };
}
