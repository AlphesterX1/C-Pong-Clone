struct Button_State {
	bool isDown;
	bool changed;
};

enum {
	Up,
	Down,
	Right,
	Left,
	Space,
	W,
	A,
	S,
	D,
	Enter,

	KeyCount, // Last item
};


struct Input {
	Button_State buttons[KeyCount];
};