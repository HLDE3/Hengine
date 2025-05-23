//
// Created by ruhld on 23.05.2025.
//

#pragma once


#ifndef TYPEUTILS_H
#define TYPEUTILS_H

#include <GL/gl.h>

#include <GL/glew.h>
#include <stdexcept>

namespace type_utils {
    inline size_t size_of_gl_enum(GLenum type) {
        switch (type) {
            case GL_BYTE:              return sizeof(GLbyte);
            case GL_UNSIGNED_BYTE:     return sizeof(GLubyte);
            case GL_SHORT:             return sizeof(GLshort);
            case GL_UNSIGNED_SHORT:    return sizeof(GLushort);
            case GL_INT:               return sizeof(GLint);
            case GL_UNSIGNED_INT:      return sizeof(GLuint);
            case GL_FLOAT:             return sizeof(GLfloat);
            case GL_DOUBLE:            return sizeof(GLdouble);
            case GL_HALF_FLOAT:        return sizeof(float) / 2;
            case GL_FIXED:             return sizeof(GLfixed);

            // Упакованные пиксельные форматы
            case GL_UNSIGNED_SHORT_5_6_5:
            case GL_UNSIGNED_SHORT_4_4_4_4:
            case GL_UNSIGNED_SHORT_5_5_5_1:
                return 2;
            case GL_UNSIGNED_INT_10_10_10_2:
            case GL_UNSIGNED_INT_24_8:
            case GL_UNSIGNED_INT_10F_11F_11F_REV:
            case GL_UNSIGNED_INT_5_9_9_9_REV:
            case GL_INT_2_10_10_10_REV:
                return 4;

            // Матричные типы (GLM)
            case GL_FLOAT_MAT2:        return sizeof(GLfloat) * 2 * 2;
            case GL_FLOAT_MAT3:        return sizeof(GLfloat) * 3 * 3;
            case GL_FLOAT_MAT4:        return sizeof(GLfloat) * 4 * 4;
            case GL_FLOAT_MAT2x3:      return sizeof(GLfloat) * 2 * 3;
            case GL_FLOAT_MAT2x4:      return sizeof(GLfloat) * 2 * 4;
            case GL_FLOAT_MAT3x2:      return sizeof(GLfloat) * 3 * 2;
            case GL_FLOAT_MAT3x4:      return sizeof(GLfloat) * 3 * 4;
            case GL_FLOAT_MAT4x2:      return sizeof(GLfloat) * 4 * 2;
            case GL_FLOAT_MAT4x3:      return sizeof(GLfloat) * 4 * 3;

            // Векторные типы (для шейдеров)
            case GL_FLOAT_VEC2:        return sizeof(GLfloat) * 2;
            case GL_FLOAT_VEC3:        return sizeof(GLfloat) * 3;
            case GL_FLOAT_VEC4:        return sizeof(GLfloat) * 4;
            case GL_INT_VEC2:          return sizeof(GLint) * 2;
            case GL_INT_VEC3:          return sizeof(GLint) * 3;
            case GL_INT_VEC4:          return sizeof(GLint) * 4;
            case GL_BOOL_VEC2:         return sizeof(GLint) * 2;
            case GL_BOOL_VEC3:         return sizeof(GLint) * 3;
            case GL_BOOL_VEC4:         return sizeof(GLint) * 4;

            // Для compute shaders
            case GL_UNSIGNED_INT_ATOMIC_COUNTER:
                return sizeof(GLuint);

            default:
                throw std::runtime_error("Unknown OpenGL type");
        }
    }
}

#endif //TYPEUTILS_H
