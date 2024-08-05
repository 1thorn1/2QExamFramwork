#pragma once

#include "BaseWindow.h"

class MainWindow : public BaseWindow
{
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

public:
    MainWindow() = default;

    virtual void Finalize() override;

};

