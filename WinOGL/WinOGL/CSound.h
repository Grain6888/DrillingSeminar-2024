#pragma once
#include "pch.h"
#include "resource.h"
#include <iostream>
#include <Windows.h>
#include <MMSystem.h>

const UINT BUTTON = 1;
const UINT LEVER = 2;
const UINT BET = 3;
const UINT GAKO = 4;

class CSound {
public:
    void test ();
    void JugglerSound (UINT name);
};

