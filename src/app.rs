use crate::config::AppConfig;
use crate::window::{Window, WindowSpecs};

#[derive(Debug)]
pub struct AppSpecs {
    pub config: AppConfig,
}

#[derive(Debug)]
pub struct App {
    config: AppConfig,
    window: Window,
    is_running: bool,
}

impl App {

    pub fn new(specs: AppSpecs) -> Self {
        let window = Window::new(WindowSpecs { 
            width: specs.config.window.size.width,
            height: specs.config.window.size.width,
            x: specs.config.window.pos.x,
            y: specs.config.window.pos.y,
         });
        
        App {
            config: specs.config,
            window: window,
            is_running: true,
        }
    }

    pub fn run(&mut self) {
        while self.is_running {
            self.window.update();
        }
    }
}
