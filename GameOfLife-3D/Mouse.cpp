#include "Mouse.h"

/*********** EVENT DEFINITIONS ***********/
Mouse::Event::Event( Type type,const Mouse& parent )
    :
    type( type ),
    x( parent.x ),
    y( parent.y ),
    leftIsPressed( parent.leftIsPressed ),
    rightIsPressed( parent.rightIsPressed )
{}

Mouse::Event::Type Mouse::Event::GetType() const noexcept
{
    return type;
}

std::pair<int,int> Mouse::Event::GetPos() const noexcept
{
    return std::pair<int,int>{ x,y };
}

int Mouse::Event::GetPosX() const noexcept
{
    return x;
}

int Mouse::Event::GetPosY() const noexcept
{
    return y;
}

bool Mouse::Event::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

bool Mouse::Event::RightIsPressed() const noexcept
{
    return rightIsPressed;
}
/********* EVENT DEFINITIONS END *********/

std::pair<int,int> Mouse::GetPos() const noexcept
{
    return std::pair<int,int>{ x,y };
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
    if ( !IsEmpty() )
    {
        auto e = buffer.front();
        buffer.pop();
        return e;
    }
    return std::optional<Event>{};
}

bool Mouse::IsEmpty() const noexcept
{
    return buffer.empty();
}

void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>{};
}

void Mouse::OnMouseMove( int x,int y ) noexcept
{
    this->x = x;
    this->y = y;

    buffer.push( Event{ Event::Type::Move,*this } );
    TrimBuffer();
}

void Mouse::OnLeftPressed( int x,int y ) noexcept
{
    leftIsPressed = true;

    buffer.push( Event{ Event::Type::LPress,*this } );
    TrimBuffer();
}

void Mouse::OnLeftReleased( int x,int y ) noexcept
{
    leftIsPressed = false;

    buffer.push( Event{ Event::Type::LRelease,*this } );
    TrimBuffer();
}

void Mouse::OnRightPressed( int x,int y ) noexcept
{
    rightIsPressed = true;

    buffer.push( Event{ Event::Type::RPress,*this } );
    TrimBuffer();
}

void Mouse::OnRightReleased( int x,int y ) noexcept
{
    rightIsPressed = false;

    buffer.push( Event{ Event::Type::RRelease,*this } );
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while ( buffer.size() > bufferSize )
    {
        buffer.pop();
    }
}
