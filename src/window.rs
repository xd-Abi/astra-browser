use std::{ptr, mem};
use windows_sys::{
    core::*, Win32::Foundation::*, Win32::Graphics::Gdi::*,
    Win32::System::LibraryLoader::GetModuleHandleA, Win32::UI::WindowsAndMessaging::*,
};

#[derive(Debug)]
pub struct WindowSpecs {
    pub width: u32,
    pub height: u32,
    pub x: i32,
    pub y: i32,
}

#[derive(Debug)]
pub struct Window {
    width: u32,
    height: u32,
    handle: HWND,
}

static mut WINDOW_COUNT: usize = 0;

impl Window {

    pub fn new(specs: WindowSpecs) -> Self {
        let handle: HWND;

        unsafe {
            let instance_handle = GetModuleHandleA(ptr::null());

            if instance_handle == 0 {
                MessageBoxA(
                    0,                   
                    s!("Invalid Instance Handle!"),  
                    s!("Error"), 
                    MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
                );
                panic!("Invalid Instance Handle!");
            }

            if WINDOW_COUNT == 0 {
                let wc = WNDCLASSA {
                    hCursor: LoadCursorW(0, IDC_ARROW),
                    hInstance: instance_handle,
                    lpszClassName: s!("AstraWindow"),
                    style: CS_HREDRAW | CS_VREDRAW,
                    lpfnWndProc: Some(process_message),
                    cbClsExtra: 0,
                    cbWndExtra: 0,
                    hIcon: 0,
                    hbrBackground: 0,
                    lpszMenuName: ptr::null(),
                };
        
                if RegisterClassA(&wc) == 0 {
                    MessageBoxA(
                        0,                   
                        s!("Window Class Registration Failed!"), 
                        s!("Error"), 
                        MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
                    );
                    panic!("Window Class Registration Failed!");
                }
            }

            handle = CreateWindowExA(
                0,
                s!("AstraWindow"),
                s!("Astra"),
                WS_OVERLAPPEDWINDOW,
                specs.x,
                specs.y,
                specs.width as i32,
                specs.height as i32,
                0,
                0,
                instance_handle,
                std::ptr::null(),
            );

            if handle == 0 {
                MessageBoxA(
                    0,                   
                    s!("Window Creation Failed!"),   
                    s!("Error"), 
                    MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
                );
                panic!("Window Creation Failed!");
    
            }

            ShowWindow(handle, SW_SHOWNORMAL);
            UpdateWindow(handle);
            WINDOW_COUNT += 1;
        }

        Window {
            width: specs.width,
            height: specs.height,
            handle: handle
        }
    }

    pub fn update(&self) {
        unsafe {
            let mut msg = mem::zeroed();
    
            while PeekMessageA(&mut msg, self.handle, 0, 0, PM_REMOVE) != 0 {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
    } 

    pub fn get_width(&self) -> u32 {
        return self.width;
    }

    pub fn get_height(&self) -> u32 {
        return self.height;
    }

    pub fn get_handle(&self) -> HWND {
        return self.handle;
    }
}

impl Drop for Window {

    fn drop(&mut self) {
        unsafe {
            if self.handle != 0 {
                DestroyWindow(self.handle);
            }

            WINDOW_COUNT -= 1;

            if WINDOW_COUNT == 0 {
                UnregisterClassA(s!("AstraWindow"), GetModuleHandleA(ptr::null()));
            }
        }
    }
}

extern "system" fn process_message(handle: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {
    unsafe {

        match message {
            _ => DefWindowProcA(handle, message, wparam, lparam)
        }
    }
}