#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

#include "Credential/Credential.h"

#include "View/ViewBase.h"
#include "View/PlatformView.h"

PlatformView::PlatformView(bnb::platform_tree::data_type& tp, QWidget * parent)
	: ViewBase(tp.m_Key.m_ID, parent)
	, m_Platform(tp)
{
	
}

PlatformView::~PlatformView() {
	
}

void PlatformView::Initialize()
{
	



}
