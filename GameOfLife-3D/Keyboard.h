#pragma once
#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release
		};
	public:
		Event( Type type,unsigned char code );
		bool IsPress() const noexcept;
		bool IsRelease() const noexcept;
		unsigned char GetCode() const noexcept;
	private:
		Type type;
		unsigned char keycode;
	};
public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;

	bool IsKeyPressed( unsigned char keycode ) const noexcept;
	std::optional<Event> ReadKey() noexcept;
	bool IsKeyEmpty() const noexcept;
	void FlushKey() noexcept;
	std::optional<char> ReadChar() noexcept;
	bool IsCharEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;

	void EnableAutorepeate() noexcept;
	void DisableAutorepeat() noexcept;
	bool IsAutorepeatEnabled() const noexcept;
private:
	void OnKeyPressed( unsigned char keycode ) noexcept;
	void OnKeyReleased( unsigned char keycode ) noexcept;
	void ClearState() noexcept;
	void OnChar( char c ) noexcept;

	template<typename T>
	void TrimBuffer( std::queue<T> buffer ) noexcept
	{
		while ( buffer.size() > bufferSize )
		{
			buffer.pop();
		}
	}
private:
	static constexpr unsigned short nKeys = 256u;
	static constexpr unsigned short bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::queue<Event> keyBuffer;
	std::bitset<nKeys> keyMap;
	std::queue<char> charBuffer;
};