#pragma once

#include <iostream>

namespace Magma
{
	/// <summary>
	///		Interface class used to implement serialization and deserialization
	/// </summary>
	class Serializable
	{
	public:
		/// <summary>
		///		Serializes this object into a stream
		/// </summary>
		/// <param name="stream">Stream where the object will be serialized to</param>
		virtual void Serialize(std::ostream& stream) const = 0;

		/// <summary>
		///		Deserializes this object from a stream
		/// </summary>
		/// <param name="stream">Stream where the object will be deserialized from</param>
		virtual void Deserialize(std::istream& stream) = 0;
	};

	inline std::ostream& operator<<(std::ostream& ostream, const Serializable& serializable)
	{
		serializable.Serialize(ostream);
		return ostream;
	}

	inline std::istream& operator>>(std::istream& istream, Serializable& serializable)
	{
		serializable.Deserialize(istream);
		return istream;
	}
}