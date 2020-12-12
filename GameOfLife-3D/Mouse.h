#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			Move
		};
	public:
		Event( Type type,const Mouse& parent );
		Type GetType() const noexcept;
		std::pair<int,int> GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool LeftIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;

	private:
		Type type;
		int x;
		int y;
		bool leftIsPressed;
		bool rightIsPressed;
	};
public:
	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	std::pair<int,int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	std::optional<Event> Read() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;
private:
	void OnMouseMove( int x,int y ) noexcept;
	void OnLeftPressed( int x,int y ) noexcept;
	void OnLeftReleased( int x,int y ) noexcept;
	void OnRightPressed( int x,int y ) noexcept;
	void OnRightReleased( int x,int y ) noexcept;
	void TrimBuffer() noexcept;
private:
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> buffer;
};