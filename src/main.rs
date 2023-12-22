use once_cell::unsync::OnceCell;
use std::mem;
use std::ptr;
use std::rc::Rc;
use webview2::Controller;
use windows_sys::{
    core::*, Win32::Foundation::*, Win32::Graphics::Gdi::*,
    Win32::System::LibraryLoader::GetModuleHandleA, Win32::UI::WindowsAndMessaging::*,
};

fn main() {
    // Check if WebView2 Runtime is available.
    //
    // @TODO: Install the WebView2 Runtime if it is not available
    // on the operating system via rust-http for example.
    if webview2::get_available_browser_version_string(None).is_err() { 
        unsafe {
            MessageBoxA(
                0,                   
                s!("WebView2 Runtime is not available!"), 
                s!("Error"),  
                MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
            );
        }

        return;
    }

    unsafe {
        let instance = GetModuleHandleA(ptr::null());
        
        if instance == 0 {
            MessageBoxA(
                0,                   
                s!("Invalid Instance Handle!"),  
                s!("Error"), 
                MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
            );
    
            return;
        }

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
            lpszMenuName: ptr::null(),
        };

        if RegisterClassA(&wc) == 0 {
            MessageBoxA(
                0,                   
                s!("Window Class Registration Failed!"), 
                s!("Error"), 
                MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
            );
    
            return;
        }

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

        if window == 0 {
            MessageBoxA(
                0,                   
                s!("Window Creation Failed!"),   
                s!("Error"), 
                MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
            );
    
            return;
        }

        ShowWindow(window, SW_SHOWNORMAL);
        UpdateWindow(window);

        // Create the webview environment
        let controller = Rc::new(OnceCell::<Controller>::new());
        let controller_clone = controller.clone();

        let result = webview2::Environment::builder().build(move |env| {
            env.unwrap().create_controller(mem::transmute(window), move |controller| {
                let controller = controller.unwrap();
                let mut client_rect = mem::zeroed();

                GetClientRect(window, &mut client_rect);
                controller.put_bounds(mem::transmute(client_rect)).unwrap();

                let web_view = controller.get_webview().unwrap();
                web_view.navigate("https://www.google.com").unwrap();
                
                controller_clone.set(controller).unwrap();
                SetWindowLongPtrA(window, GWLP_USERDATA, mem::transmute(controller_clone.as_ref()));

                Ok(())
            })
        });

        if let Err(_e) = result {
            MessageBoxA(
                0,                   
                s!("Web View Creation Failed!"),   
                s!("Error"), 
                MB_OK | MB_ICONERROR | MB_SYSTEMMODAL,
            );
    
            return;
        }

        let mut message = std::mem::zeroed();

        while GetMessageA(&mut message, 0, 0, 0) != 0 {
            DispatchMessageA(&message);
        }
    }
}

extern "system" fn process_message(window: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {
    unsafe {
        match message {
            WM_SIZE => {
                let user_data = GetWindowLongPtrA(window, GWLP_USERDATA);
                if user_data != 0 {
                    let controller: &OnceCell<Controller> = mem::transmute(user_data);

                    if let Some(c) = controller.get() {
                        let mut client_rect = mem::zeroed();
                        GetClientRect(window, &mut client_rect);

                        c.put_bounds(mem::transmute(client_rect)).unwrap();
                    }
                    
                }
              0
            }
            WM_DESTROY => {
                PostQuitMessage(0);
                0
            }
            _ => DefWindowProcA(window, message, wparam, lparam)
        }
    }
}