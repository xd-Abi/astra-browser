use std::fs;
use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct AppConfig {
    pub window: WindowConfig
}

#[derive(Debug, Deserialize)]
pub struct WindowConfig {
    pub pos: WindowPositionConfig,
    pub size: WindowSizeConfig,
}

#[derive(Debug, Deserialize)]
pub struct WindowPositionConfig {
    pub x: i32,
    pub y: i32,
}

#[derive(Debug, Deserialize)]
pub struct WindowSizeConfig {
    pub width: u32,
    pub height: u32,
}

impl AppConfig {

    pub fn new() -> Self {
        let content = fs::read_to_string("settings.ini")
            .expect("Failed to read settings.ini");

        let config: AppConfig = toml::from_str(&content)
            .expect("Failed to parse TOML");

        config
    }
}