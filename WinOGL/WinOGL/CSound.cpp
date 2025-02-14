#include "pch.h"
#include "CSound.h"

void CSound::test ()
{
    HINSTANCE hInstance = AfxGetInstanceHandle ();
    if (!PlaySound (MAKEINTRESOURCE (IDR_LEVER), hInstance, SND_RESOURCE | SND_ASYNC))
    {
        PlaySound (TEXT ("C:\\windows\\Media\\notify.wav"), NULL, (SND_ASYNC | SND_FILENAME));
    }
}

void CSound::JugglerSound (UINT name)
{
    HINSTANCE hInstance = AfxGetInstanceHandle ();
    switch (name)
    {
        case BUTTON:
            PlaySound (MAKEINTRESOURCE (IDR_BUTTON), hInstance, SND_RESOURCE | SND_ASYNC);
            break;
        case LEVER:
            PlaySound (MAKEINTRESOURCE (IDR_LEVER), hInstance, SND_RESOURCE | SND_ASYNC);
            break;
        case BET:
            PlaySound (MAKEINTRESOURCE (IDR_BET), hInstance, SND_RESOURCE | SND_ASYNC);
            break;
        default:
            break;
    }
}
