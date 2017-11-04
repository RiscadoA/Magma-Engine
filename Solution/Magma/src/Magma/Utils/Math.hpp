#pragma once

#include <glm\glm.hpp>

// Adds serializing and deserializing functions to GLM types

#define MAGMA_DETAIL_DEFINE_IO(vecType)\
std::istream& operator>>(std::istream& stream, vecType& v)\
{\
	for (size_t i = 0; i < v.length(); ++i)\
		stream >> v[i];\
    return stream;\
}\
std::ostream& operator<<(std::ostream& stream, const vecType& v)\
{\
	for (size_t i = 0; i < v.length(); ++i)\
		stream << v[i] << ' ';\
    return stream;\
}

MAGMA_DETAIL_DEFINE_IO(glm::vec2);
MAGMA_DETAIL_DEFINE_IO(glm::vec3);
MAGMA_DETAIL_DEFINE_IO(glm::vec4);
MAGMA_DETAIL_DEFINE_IO(glm::mat2);
MAGMA_DETAIL_DEFINE_IO(glm::mat3);
MAGMA_DETAIL_DEFINE_IO(glm::mat4);

#undef MAGMA_DETAIL_DEFINE_IO