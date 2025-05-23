
#include "FrameBuffer.h"
#include <cstdio>
#include <GL/glew.h>

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &framebuffer);
}

FrameBuffer::FrameBuffer(int w, int h) : width(w), height(h) {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // --- Multisample color renderbuffer ---
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

    // --- (Optional) Multisample depth renderbuffer ---
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is not complete!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    if (colorRenderbuffer) glDeleteRenderbuffers(1, &colorRenderbuffer);
    if (depthRenderbuffer) glDeleteRenderbuffers(1, &depthRenderbuffer);
    if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
}

void FrameBuffer::bind_write() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::unbind_write() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind_read() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::unbind_read() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    if (colorRenderbuffer) glDeleteRenderbuffers(1, &colorRenderbuffer);
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

    if (depthRenderbuffer) glDeleteRenderbuffers(1, &depthRenderbuffer);
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::clear() const {
    bind_write();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    unbind_write();
}