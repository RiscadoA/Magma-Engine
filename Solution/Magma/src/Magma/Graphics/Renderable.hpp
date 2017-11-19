#pragma once

namespace Magma
{
	/// <summary>
	///		Abstract classed inherited by objects that can be rendered
	/// </summary>
	class Renderable
	{
	public:
		/// <summary>
		///		Renders this renderable
		/// </summary>
		virtual void Render() = 0;
	};
}