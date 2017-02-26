
#ifndef WINTAB_FUNCTIONALITY_HPP
#define WINTAB_FUNCTIONALITY_HPP

///
/// Stuff for Windows WinTab Functionality:
///
//// TODO: Move to better place
///

//
// Modified from stdafx.h
//
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//
// Modified from PressureTest.cpp
//
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#include "tablet/msgpack.h"
#include "tablet/wintab.h"
#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE	PK_BUTTONS
#include "tablet/pktdef.h"
#include "tablet/Utils.h"

#define MAX_LOADSTRING 100

// JUST so that tablet functionality will stop complaining
// about undefined external reference.
// TODO: Fix this later...
LPCTSTR gpszProgramName = "PressureTest";
static LOGCONTEXT	glogContext = {0};


HCTX init_tablet_context(HWND hWnd)
{
    HCTX hCtx = NULL;

    /// Commented because unused
    ///UINT wDevice = 0;
    ///UINT wExtX = 0;
    ///UINT wExtY = 0;
    UINT wWTInfoRetVal = 0;
    AXIS
     TabletX = {0};
    AXIS TabletY = {0};

    // Set option to move system cursor before getting default system context.
    glogContext.lcOptions |= CXO_SYSTEM;

    // Open default system context so that we can get tablet data
    // in screen coordinates (not tablet coordinates).
    wWTInfoRetVal = gpWTInfoA(WTI_DEFSYSCTX, 0, &glogContext);
    WACOM_ASSERT( wWTInfoRetVal == sizeof( LOGCONTEXT ) );

    WACOM_ASSERT( glogContext.lcOptions & CXO_SYSTEM );

    // modify the digitizing region
    //////////////////////////////////wsprintf(glogContext.lcName, "PrsTest Digitizing %x", hInst);

    // We process WT_PACKET (CXO_MESSAGES) messages.
    glogContext.lcOptions |= CXO_MESSAGES;

    // What data items we want to be included in the tablet packets
    glogContext.lcPktData = PACKETDATA;

    // Which packet items should show change in value since the last
    // packet (referred to as 'relative' data) and which items
    // should be 'absolute'.
    glogContext.lcPktMode = PACKETMODE;

    // This bitfield determines whether or not this context will receive
    // a packet when a value for each packet field changes.  This is not
    // supported by the Intuos Wintab.  Your context will always receive
    // packets, even if there has been no change in the data.
    glogContext.lcMoveMask = PACKETDATA;

    // Which buttons events will be handled by this context.  lcBtnMask
    // is a bitfield with one bit per button.
    glogContext.lcBtnUpMask = glogContext.lcBtnDnMask;

    // Set the entire tablet as active
    wWTInfoRetVal = gpWTInfoA( WTI_DEVICES + 0, DVC_X, &TabletX );
    WACOM_ASSERT( wWTInfoRetVal == sizeof( AXIS ) );

    wWTInfoRetVal = gpWTInfoA( WTI_DEVICES, DVC_Y, &TabletY );
    WACOM_ASSERT( wWTInfoRetVal == sizeof( AXIS ) );

    glogContext.lcInOrgX = 0;
    glogContext.lcInOrgY = 0;
    glogContext.lcInExtX = TabletX.axMax;
    glogContext.lcInExtY = TabletY.axMax;

    // Guarantee the output coordinate space to be in screen coordinates.
    glogContext.lcOutOrgX = GetSystemMetrics( SM_XVIRTUALSCREEN );
    glogContext.lcOutOrgY = GetSystemMetrics( SM_YVIRTUALSCREEN );
    glogContext.lcOutExtX = GetSystemMetrics( SM_CXVIRTUALSCREEN ); //SM_CXSCREEN );

    // In Wintab, the tablet origin is lower left.  Move origin to upper left
    // so that it coincides with screen origin.
    glogContext.lcOutExtY = -GetSystemMetrics( SM_CYVIRTUALSCREEN );	//SM_CYSCREEN );

    // Leave the system origin and extents as received:
    // lcSysOrgX, lcSysOrgY, lcSysExtX, lcSysExtY

    // open the region
    // The Wintab spec says we must open the context disabled if we are
    // using cursor masks.
    //hctx = gpWTOpenA( hWnd, &glogContext, FALSE );
    //hCtx = gpWTOpenA( NULL, &glogContext, FALSE ); // TRUE?
    hCtx = gpWTOpenA(hWnd, &glogContext, FALSE ); // TRUE?

    WacomTrace("HCTX: %i\n", hCtx);

    gpWTEnable(hCtx, TRUE);
    if (hCtx && TRUE)
    {
        gpWTOverlap(hCtx, TRUE);
    }
    return hCtx;
}

/**
 * hCtx is the tablet context, should be declared outside of the function,
 * and is initialized by this function to be used with the other WinTab functions
 */
bool init_tablet(HCTX& hCtx, sf::RenderWindow& window)
{
    if (!LoadWintab())
	{
		ShowError( "Wintab not available" );
		return FALSE;
	}

	// check if WinTab available.
	if (!gpWTInfoA(0, 0, NULL))
	{
	    // Uncomment to prevent annoyance
		//ShowError( "WinTab Services Not Available." );
		return false;
	}
	// get tablet context
    //HCTX hCtx = init_tablet_context(window.getSystemHandle());
    hCtx = init_tablet_context(window.getSystemHandle());
    if (!hCtx)
    {
        ShowError("Could Not Open Tablet Context.");
        return false;
    }

    return true;
}
/*
    double pressure = -1.0;
    int numDevices = 0;

    sf::CircleShape circle;
    circle.setFillColor(sf::Color(0, 200, 100));
    circle.setRadius(0);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    while (true)
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(circle);
        window.display();

        // Real-time get number of devices connected:
        if (!gpWTInfoA(0, 0, NULL))
        {
            numDevices = 0;
        }

        gpWTInfoA(WTI_INTERFACE, IFC_NDEVICES, &numDevices);
        //std::cout << "numDevices = " << numDevices << std::endl;

        // Tablet Packet Information:
        PACKET pkt;

       // std::cout << "start WTPacketsGet()" << std::endl;
        if (gpWTPacketsGet(hCtx, 1, &pkt))
        {
            //std::cout << "end   WTPacketsGet()" << std::endl;
            pressure = pkt.pkNormalPressure;
            circle.setRadius(pressure / 100);

            std::cout << pressure << "!" << std::endl;
            //std::cin >> numDevices;
        }
        else
        {
            //std::cout << "end   WTPacketsGet()" << std::endl;

            //std::cout << start WTPacketsGet2
            //gpWTPacketsGet(hCtx, 100, 0);
            //std::cout << "ERROR" << std::endl;
        }
    }
}
*/

/*
 * pressure will be in range [0, 2047]
 */
int getPressure(HCTX hCtx)
{
    int pressure;
    PACKET pkt;

    if (gpWTPacketsGet(hCtx, 1, &pkt))
    {
        pressure = pkt.pkNormalPressure;
    }
    else
    {
        gpWTPacketsGet(hCtx, 4*sizeof(PACKET), NULL);
        pressure = -1;
    }

    //std::cout << pressure << std::endl;

    return pressure;
}

struct TabletContext {

};


#endif // WINTAB_FUNCTIONALITY_HPP
