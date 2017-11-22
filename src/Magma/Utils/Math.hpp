#pragma once

#include <glm\glm.hpp>
#include <iostream>

#define MAGMA_DETAIL_DEFINE_IO(vecType)\
inline std::istream& operator>>(std::istream& stream, vecType& v)\
{\
	for (size_t i = 0; i < v.length(); ++i)\
		stream >> v[i];\
    return stream;\
}\
inline std::ostream& operator<<(std::ostream& stream, const vecType& v)\
{\
	for (size_t i = 0; i < v.length(); ++i)\
		stream << v[i] << ' ';\
    return stream;\
}

// Adds serializing and deserializing functions to GLM types

MAGMA_DETAIL_DEFINE_IO(glm::vec2);
MAGMA_DETAIL_DEFINE_IO(glm::vec3);
MAGMA_DETAIL_DEFINE_IO(glm::vec4);
MAGMA_DETAIL_DEFINE_IO(glm::mat2);
MAGMA_DETAIL_DEFINE_IO(glm::mat3);
MAGMA_DETAIL_DEFINE_IO(glm::mat4);
