// ButtonListButtonControl.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ButtonListButtonControl.h"


// ButtonListButtonControl

IMPLEMENT_DYNAMIC(ButtonListButtonControl, CButton)

ButtonListButtonControl::ButtonListButtonControl()
{

}

ButtonListButtonControl::~ButtonListButtonControl()
{
}


BEGIN_MESSAGE_MAP(ButtonListButtonControl, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &ButtonListButtonControl::OnBnClicked)
END_MESSAGE_MAP()



// ButtonListButtonControl message handlers


#include "ButtonListControl.h"

void ButtonListButtonControl::OnBnClicked()
{
	((ButtonListControl*)GetParent())->onButton(this);
}
