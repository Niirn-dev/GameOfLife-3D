#include "Keyboard.h"

/*********** EVENT DEFINITIONS ***********/
Keyboard::Event::Event( Type type,unsigned char code )
    :
    type( type ),
    keycode( code )
{}

bool Keyboard::Event::IsPress() const noexcept
{
    return type == Type::Press;
}

bool Keyboard::Event::IsRelease() const noexcept
{
    return type == Type::Release;
}

unsigned char Keyboard::Event::GetCode() const noexcept
{
    return keycode;
}
/********* EVENT DEFINITIONS END *********/

bool Keyboard::IsKeyPressed( unsigned char code ) const noexcept
{
    return keyMap[code];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
    if ( !IsKeyEmpty() )
    {
        auto e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
    return std::optional<Event>{};
}

bool Keyboard::IsKeyEmpty() const noexcept
{
    return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    keyBuffer = std::queue<Event>{};
}

std::optional<char> Keyboard::ReadChar() noexcept
{
    if ( !IsCharEmpty() )
    {
        auto e = charBuffer.front();
        charBuffer.pop();
        return e;
    }
    return std::optional<char>{};
}

bool Keyboard::IsCharEmpty() const noexcept
{
    return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
    charBuffer = std::queue<char>{};
}

void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeate() noexcept
{
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
    autorepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled() const noexcept
{
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed( unsigned char keycode ) noexcept
{
    keyMap[keycode] = true;
    keyBuffer.push( Event{ Event::Type::Press,keycode } );
    TrimBuffer( keyBuffer );
}

void Keyboard::OnKeyReleased( unsigned char keycode ) noexcept
{
    keyMap[keycode] = false;
    keyBuffer.push( Event{ Event::Type::Release,keycode } );
    TrimBuffer( keyBuffer );
}

void Keyboard::ClearState() noexcept
{
    keyMap.reset();
}

void Keyboard::OnChar( char c ) noexcept
{
    charBuffer.push( c );
    TrimBuffer( charBuffer );
}
