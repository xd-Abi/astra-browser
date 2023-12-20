#pragma once

#include <astra/common.h>

namespace Astra {

	class GraphicsContext
	{
	public:
		/**
		 * @brief Constructor
		 * @param handle The window handle.
		 */
		GraphicsContext(HWND handle);

		/**
		 * @brief Destructor
		 */
		~GraphicsContext();

		/**
		 * @brief Swaps the front and back buffers, presenting the rendered content.
		 *
		 * This method is responsible for swapping the front and back buffers,
		 * presenting the rendered content to the screen.
		 */
		void SwapBuffers();

	private:
		HGLRC m_OpenGLContext;
		HDC m_Hdc;
	};
}