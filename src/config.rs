use std::fs;
use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct AppConfig {
    window: WindowConfig
}

#[derive(Debug, Deserialize)]
pub struct WindowConfig {
    pos: WindowPositionConfig,
    size: WindowSizeConfig,
}

#[derive(Debug, Deserialize)]
pub struct WindowPositionConfig {
    x: i32,
    y: i32,
}

#[derive(Debug, Deserialize)]
pub struct WindowSizeConfig {
    width: u32,
    height: u32,
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