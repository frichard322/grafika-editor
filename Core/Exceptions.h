#pragma once

#include <iostream>
#include <string>
#include <QMessageBox>

namespace cagd
{
    class Exception
    {
        friend std::ostream& operator <<(std::ostream& lhs, const Exception& rhs);

    protected:
        std::string _reason;

    public:
        Exception(const std::string &reason): _reason(reason)
        {
        }

        void what() const
        {
            QMessageBox::critical(nullptr, "An exception has occured...", QString::fromStdString(_reason));
        }
    };

    inline std::ostream& operator <<(std::ostream& lhs, const Exception& rhs)
    {
        return lhs << rhs._reason;
    }
}
