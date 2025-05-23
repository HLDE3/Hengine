//
// Created by ruhld on 22.05.2025.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H



class FrameBuffer {
public:
    unsigned int framebuffer = 0;
    unsigned int colorRenderbuffer = 0;
    unsigned int depthRenderbuffer = 0;
    int width = 0, height = 0;

    FrameBuffer();
    FrameBuffer(int w, int h);
    ~FrameBuffer();

    void bind_write() const;
    void unbind_write() const;
    void bind_read() const;
    void unbind_read() const;
    void resize(int newWidth, int newHeight);
    void clear() const;
};



#endif //FRAMEBUFFER_H
