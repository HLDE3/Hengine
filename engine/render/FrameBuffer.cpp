#include "FrameBuffer.h"
#include <cstdio>
#include <GL/glew.h>

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &framebuffer);
}

FrameBuffer::FrameBuffer(int w, int h) : width(w), height(h) {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Цветовая текстура
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Глубинный рендербуфер
    glGenRenderbuffers(1, &depth_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is not complete!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    if (depth_renderbuffer) glDeleteRenderbuffers(1, &depth_renderbuffer);
    if (texture) glDeleteTextures(1, &texture);
    if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
}

void FrameBuffer::bind_write() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::unbind_write() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBuffer::bind_read() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::unbind_read() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::bind_texture() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void FrameBuffer::unbind_texture() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    if (texture) glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (depth_renderbuffer) glDeleteRenderbuffers(1, &depth_renderbuffer);
    glGenRenderbuffers(1, &depth_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::clear() const {
    bind_write();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    unbind_write();
}

void FrameBuffer::blit() const {
    bind_read();
    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    unbind_read();
}