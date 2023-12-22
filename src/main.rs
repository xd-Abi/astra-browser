use windows_sys::{
    core::*, Win32::Foundation::*, Win32::Graphics::Gdi::*,
    Win32::System::LibraryLoader::GetModuleHandleA, Win32::UI::WindowsAndMessaging::*,
};

fn main() {
    unsafe {
        let instance = GetModuleHandleA(std::ptr::null());
        debug_assert!(instance != 0);

        let window_class: *const u8 = s!("AstraWindow");
        let wc = WNDCLASSA {
            hCursor: LoadCursorW(0, IDC_ARROW),
            hInstance: instance,
            lpszClassName: window_class,
            style: CS_HREDRAW | CS_VREDRAW,
            lpfnWndProc: Some(process_message),
            cbClsExtra: 0,
            cbWndExtra: 0,
            hIcon: 0,
            hbrBackground: 0,
            lpszMenuName: std::ptr::null(),
        };

        let atom = RegisterClassA(&wc);
        debug_assert!(atom != 0);

        let window = CreateWindowExA(
            0,
            window_class,
            s!("Astra"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            instance,
            std::ptr::null(),
        );

        ShowWindow(window, SW_SHOWNORMAL);
        UpdateWindow(window);

        let mut message = std::mem::zeroed();

        while GetMessageA(&mut message, 0, 0, 0) != 0 {
            DispatchMessageA(&message);
        }
    }
}

extern "system" fn process_message(window: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {
    unsafe {
        match message {
            WM_DESTROY => {
                PostQuitMessage(0);
                0
            }
            _ => DefWindowProcA(window, message, wparam, lparam)
        }
    }
}