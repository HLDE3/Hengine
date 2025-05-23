#pragma once

class FrameBuffer {
public:
    unsigned int texture = 0;
    unsigned int depth_renderbuffer = 0;
    unsigned int framebuffer = 0;
    int width = 0, height = 0;

    FrameBuffer();
    FrameBuffer(int w, int h);
    ~FrameBuffer();

    void bind_write() const;
    void unbind_write() const;
    void bind_read() const;
    void unbind_read() const;
    void bind_texture() const;
    void unbind_texture() const;
    void resize(int newWidth, int newHeight);
    void clear() const;
    void blit() const;
};