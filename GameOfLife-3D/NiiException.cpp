#include "NiiException.h"
#include <sstream>

NiiException::NiiException( int line,const char* file )
    :
    line( line ),
    file( file )
{}

const char* NiiException::what() const noexcept
{
    std::stringstream oss;
    oss << "[Type] " << GetType() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* NiiException::GetType() const noexcept
{
    return "Niirn Exception";
}

int NiiException::GetLine() const noexcept
{
    return line;
}

const std::string& NiiException::GetFile() const noexcept
{
    return file;
}

std::string NiiException::GetOriginString() const noexcept
{
    std::stringstream oss;
    oss << "[Line] " << line << std::endl
        << "[File] " << file;
    return oss.str();
}
