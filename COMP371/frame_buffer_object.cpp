#include "frame_buffer_object.hpp"
#include "iostream"
using namespace std;



GLuint FrameBufferObject::createFrameBuffer() {					// Create Buffer

    GLuint id;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    return id;
}

void FrameBufferObject::bind(GLuint id, int width, int height) {		// Bind to id

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glViewport(0, 0, width, height);

}

void FrameBufferObject::unbind(int w, int h) {					// Bind Default Buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, w, h);
}

GLuint FrameBufferObject::createTextureAttachment(int width, int height) {	// For reflection & refraction

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id, 0);
    return id;
}

GLuint FrameBufferObject::createDepthTextureAttachment(int width, int height){	// For Shadow Maps

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
    return id;

}


GLuint FrameBufferObject::createDepthBufferAttachment(int width, int height) {

    GLuint id;
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
    return id;

}

void FrameBufferObject::createReflectionFrameBuffer(int w,  int h) {

    reflection_id = createFrameBuffer();
    reflection_texture_id = createTextureAttachment(width, height);
    reflection_depth_buffer_id = createDepthBufferAttachment(width, height);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    check();
    unbind(w, h);

}

void FrameBufferObject::createRefractionFrameBuffer(int w,  int h) {

    refraction_id = createFrameBuffer();
    refraction_texture_id = createTextureAttachment(width, height);
    refraction_depth_buffer_id = createDepthBufferAttachment(width, height);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    check();
    unbind(w, h);

}

void FrameBufferObject::createShadowMapFrameBuffer(int w,  int h) {

    shadowmap_id = createFrameBuffer();
    shadow_map_texture_id = createDepthTextureAttachment(width, height);
    check();
    unbind(w, h);

}

void FrameBufferObject::check() {

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	cout << "Incomplete Frame Buffer" << endl;
}
