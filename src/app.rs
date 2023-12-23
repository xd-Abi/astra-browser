use crate::layer::Layer;

pub struct App {
    layers: Vec<Box<dyn Layer>>,
}

impl App {
    pub fn new() -> Self {
        App { layers: Vec::new() }
    }

    pub fn push_layer(&mut self, layer: Box<dyn Layer>) {
        layer.on_attach();
        self.layers.push(layer);
    }
}