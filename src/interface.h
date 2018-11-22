#ifndef INTERFACE_H
#define INTERFACE_H

namespace md
{
namespace interface
{
	enum class EditMode
	{
		None,
		Wall,
		Start,
		Finish,
		Erase
	};


	void Start();

	void ProcessInput();

	EditMode GetEditState();

};
}

#endif // !INTERFACE_H
