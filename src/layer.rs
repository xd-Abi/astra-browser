pub trait Layer {
    fn on_attach(&self);
    fn on_detach(&self);
}
