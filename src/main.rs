mod app;
mod config;
mod layer;
mod window;

use config::AppConfig;
use app::{AppSpecs, App};

#[cfg(windows)]
fn main() {
    let config = AppConfig::new();
    let mut app = App::new(AppSpecs { config });
    app.run();
}
